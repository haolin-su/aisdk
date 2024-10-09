#include "snpe_infer.h"

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cassert>

#include "core/tensor/tensor.h"
#include "core/model/model.h"
#include "common/log.h"
#include "utils/createUser_buffer.hpp"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "SNPE/DlSystem/StringList.hpp"
#include "SNPE/DlSystem/TensorShape.hpp"
#include "SNPE/DlSystem/IUserBuffer.hpp"
#include "SNPE/DlSystem/RuntimeList.hpp"
#include "SNPE/DlSystem/IUserBufferFactory.hpp"
#include "SNPE/DlSystem/UserBufferMap.hpp"
#include "SNPE/DlContainer/IDlContainer.hpp"
#include "SNPE/DlSystem/PlatformConfig.hpp"
#include "SNPE/SNPEBuilder.hpp"


namespace aisdk {

class SnpeInferPriv{
public:
    SnpeInferPriv(ModelPtr model)
    {
        model_ = model;
        snpe = aisdk::any_cast<std::shared_ptr<zdl::SNPE::SNPE>>(model->GetModel());
    }

    // 初始化
    int Init(std::string config);

    // 处理数据
    int Process(TensorPtr& input, TensorPtr& output);
    int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs);

    // 释放资源
    int Finalize();

    // 获取输入输出节点的数量
    int GetInputSize();
    int GetOutputSize();

    // 设置输入输出节点
    int SetInput(const std::vector<std::shared_ptr<INode>>& inputs);
    int SetOutput(const std::vector<std::shared_ptr<INode>>& outputs);

    // 获取输入输出节点
    int GetInput(std::vector<std::shared_ptr<INode>>& inputs);
    int GetOutput(std::vector<std::shared_ptr<INode>>& outputs);

    // 获取参数
    int GetParam(const std::string& key, std::string& value);
    // 设置参数
    int SetParam(const std::string& key, const std::string& value);

    // 获取硬件IP
    std::string GetIP();
    // 获取版本号
    std::string GetVersion();

    // 获取硬件IP
    // 获取输入输出tensor
    std::vector<TensorPtr> GetInputTensors();
    std::vector<TensorPtr> GetOutputTensors();

    // 设置输入输出tensor
    int SetInputTensors(std::vector<TensorPtr>& tensor);
    int SetOutputTensors(std::vector<TensorPtr>& tensor);
private:
    ModelPtr model_;

    zdl::DlSystem::UserBufferMap input_map;
    zdl::DlSystem::UserBufferMap output_map;

    std::shared_ptr<zdl::SNPE::SNPE> snpe;

    std::vector<TensorPtr> input_tensors_;
    std::vector<TensorPtr> output_tensors_;
};

// 初始化
int SnpeInferPriv::Init(std::string config)
{
    AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Init");
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    bool staticQuantization = false;
    int bitWidth = 32;
    zdl::DlSystem::UserBufferMap input_map;
    std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpe_user_backed_input_buffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationInputBuffers;
    createInputBufferMap(input_map, applicationInputBuffers, snpe_user_backed_input_buffers, snpe, true, staticQuantization, bitWidth);

    input_tensors_ = model_->GetInputTensors();
    output_tensors_ = model_->GetOutputTensors();

    // TODO 给input/output设置tensor，这样上一个节点可以通过GetInputTensor获取tensor，处理后的结果直接写入对应的内存，下一个节点通过GetOutputTensor获取tensor，直接读取数据即可。

    return 0;
}

// 处理数据
int SnpeInferPriv::Process(TensorPtr& input, TensorPtr& output)
{
    // 检查input是否为空
    if (input == nullptr)
    {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process input is nullptr");
        return -1;
    }

    // 检查output是否为空
    if (output == nullptr)
    {
        // TODO: 创建output
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process output is nullptr");
        return -1;
    }

    // 将input数据封装为input_map
    zdl::DlSystem::UserBufferMap input_map;
    zdl::DlSystem::UserBufferMap output_map;

    void *input_data = nullptr;
    if ( 0 != input->GetData(input_data)) {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process input->GetData failed");
        return -1;
    }

    {
        std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpe_user_backed_input_buffers;

        size_t buf_size;
        input->GetDataSize(buf_size);
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process input data size: {}", buf_size);

        // std::vector<int64_t> strides = {0};
        // zdl::DlSystem::TensorShape strides ;
        TensorShape input_shape;
        input_tensors_[0]->GetShape(input_shape);

        std::vector<size_t> strides(input_shape.dim);
        strides[strides.size() - 1] = sizeof(float);
        size_t stride = strides[strides.size() - 1];
        for (size_t i = input_shape.dim - 1; i > 0; i--)
        {
            stride *= input_shape.shape[i];
            strides[i-1] = stride;
        }
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process input strides: [{}, {}, {}, {}]", strides[0], strides[1], strides[2], strides[3]);

        std::vector<std::string> names = model_->GetInputName();
        std::string name = names[0];
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process input name: %s", name.c_str());

        // create SNPE user buffer from the user-backed buffer
        zdl::DlSystem::UserBufferEncodingFloat userBufferEncodingFloat;
        zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
        // void *buffer,
        //                                             size_t bufSize,
        //                                             const TensorShape &strides,
        //                                             UserBufferEncoding* userBufferEncoding,
        //                                             UserBufferSource* userBufferSource
        snpe_user_backed_input_buffers.push_back(ubFactory.createUserBuffer(input_data,
                                                                    buf_size,
                                                                    strides,
                                                                    &userBufferEncodingFloat, nullptr));
        if (snpe_user_backed_input_buffers.back() == nullptr)
        {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process createUserBuffer failed");
            return -1;
        }
        // add the user-backed buffer to the input_map, which is later on fed to the network for execution
        input_map.add(name.c_str(), snpe_user_backed_input_buffers.back().get());
    }
    // 将output数据封装为output_map
    
    {
        
        std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpe_user_backed_output_buffers;

        size_t buf_size;
        output->GetDataSize(buf_size);

        // std::vector<int64_t> strides = {0};
        // zdl::DlSystem::TensorShape strides ;
        TensorShape output_shape;
        output_tensors_[0]->GetShape(output_shape);

        std::vector<size_t> strides(output_shape.dim);
        strides[strides.size() - 1] = sizeof(float);
        size_t stride = strides[strides.size() - 1];
        for (size_t i = output_shape.dim - 1; i > 0; i--)
        {
            stride *= output_shape.shape[i];
            strides[i-1] = stride;
        }
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process output strides: [{}, {}, {}, {}]", strides[0], strides[1], strides[2], strides[3]);

        std::vector<std::string> names = model_->GetOutputName();
        std::string name = names[0];
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process output name: {}", name);

        void *output_data = nullptr;
        if ( 0 != output->GetData(output_data)) {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process output->GetData failed");
            return -1;
        }

        // create SNPE user buffer from the user-backed buffer
        zdl::DlSystem::UserBufferEncodingFloat userBufferEncodingFloat;
        zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
        snpe_user_backed_output_buffers.push_back(ubFactory.createUserBuffer(output_data,
                                                                    buf_size,
                                                                    strides,
                                                                    &userBufferEncodingFloat, nullptr));
        if (snpe_user_backed_output_buffers.back() == nullptr)
        {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process createUserBuffer failed");
            return -1;
        }
        // add the user-backed buffer to the output_map, which is later on fed to the network for execution
        output_map.add(name.c_str(), snpe_user_backed_output_buffers.back().get());
    }

    // 执行推理
    if ( false == snpe->execute(input_map, output_map))
    {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process execute failed");
        return -1;
    }

    return 0;
}

int SnpeInferPriv::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs)
{
    if (inputs.size() != input_tensors_.size())
    {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process inputs size not match");
        return -1;
    }

    if (outputs.size() != output_tensors_.size())
    {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process outputs size not match");
        return -1;
    }

    // 将input数据封装为input_map
    zdl::DlSystem::UserBufferMap input_map;
    zdl::DlSystem::UserBufferMap output_map;

    for (size_t i = 0; i < inputs.size(); i++)
    {
        std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpe_user_backed_input_buffers;

        size_t buf_size;
        inputs[i]->GetDataSize(buf_size);

        TensorShape input_shape;
        input_tensors_[i]->GetShape(input_shape);
        
        void *input_data = nullptr;
        if ( 0 != inputs[i]->GetData(input_data)) {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process input->GetData failed");
            return -1;
        }

        std::vector<size_t> strides(input_shape.dim);
        strides[strides.size() - 1] = sizeof(float);
        size_t stride = strides[strides.size() - 1];
        for (size_t i = input_shape.dim - 1; i > 0; i--)
        {
            stride *= input_shape.shape[i];
            strides[i-1] = stride;
        }

        std::vector<std::string> names = model_->GetOutputName();
        std::string name = names[i];
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process input name: {}", name);

        zdl::DlSystem::UserBufferEncodingFloat userBufferEncodingFloat;
        zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
        snpe_user_backed_input_buffers.push_back(ubFactory.createUserBuffer(input_data,
                                                                    buf_size,
                                                                    strides,
                                                                    &userBufferEncodingFloat, nullptr));
        if (snpe_user_backed_input_buffers.back() == nullptr)
        {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process createUserBuffer failed");
            return -1;
        }
        // add the user-backed buffer to the input_map, which is later on fed to the network for execution
        input_map.add(name.c_str(), snpe_user_backed_input_buffers.back().get());
    }

    // 将output数据封装为output_map
    for (size_t i = 0; i < outputs.size(); i++)
    {
        std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpe_user_backed_output_buffers;

        size_t buf_size;
        outputs[i]->GetDataSize(buf_size);

        TensorShape output_shape;
        output_tensors_[i]->GetShape(output_shape); 

        void *output_data = nullptr;
        if ( 0 != outputs[i]->GetData(output_data)) {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process output->GetData failed");
            return -1;
        }

        std::vector<size_t> strides(output_shape.dim);
        strides[strides.size() - 1] = sizeof(float);
        size_t stride = strides[strides.size() - 1];
        for (size_t i = output_shape.dim - 1; i > 0; i--)
        {
            stride *= output_shape.shape[i];
            strides[i-1] = stride;
        }

        std::vector<std::string> names = model_->GetOutputName();
        std::string name = names[i];
        AISDK_LOG_TRACE("[registry] [snpe] SnpeInferPriv::Process output name: {}", name);

        zdl::DlSystem::UserBufferEncodingFloat userBufferEncodingFloat;
        zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
        snpe_user_backed_output_buffers.push_back(ubFactory.createUserBuffer(output_data,
                                                                    buf_size,
                                                                    strides,
                                                                    &userBufferEncodingFloat, nullptr));
        if (snpe_user_backed_output_buffers.back() == nullptr)
        {
            AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process createUserBuffer failed");
            return -1;
        }
        // add the user-backed buffer to the output_map, which is later on fed to the network for execution
        output_map.add(name.c_str(), snpe_user_backed_output_buffers.back().get());
    }

    // 执行推理
    if ( false == snpe->execute(input_map, output_map))
    {
        AISDK_LOG_ERROR("[registry] [snpe] SnpeInferPriv::Process execute failed");
        return -1;
    }

    return 0;
}


// 释放资源
int SnpeInferPriv::Finalize()
{
    return 0;
}


// 获取输入输出节点的数量
int SnpeInferPriv::GetInputSize()
{
    return 0;
}

int SnpeInferPriv::GetOutputSize()
{
    return 0;
}


// 设置输入输出节点
int SnpeInferPriv::SetInput(const std::vector<std::shared_ptr<INode>>& inputs)
{
    return 0;
}

int SnpeInferPriv::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs)
{
    return 0;
}


// 获取输入输出节点
int SnpeInferPriv::GetInput(std::vector<std::shared_ptr<INode>>& inputs)
{
    return 0;
}

int SnpeInferPriv::GetOutput(std::vector<std::shared_ptr<INode>>& outputs)
{
    return 0;
}


// 获取参数
int SnpeInferPriv::GetParam(const std::string& key, std::string& value)
{
    return 0;
}

// 设置参数
int SnpeInferPriv::SetParam(const std::string& key, const std::string& value)
{
    return 0;
}


// 获取硬件IP
std::string SnpeInferPriv::GetIP()
{
    return 0;
}

// 获取版本号
std::string SnpeInferPriv::GetVersion()
{
    return 0;
}


// 获取硬件IP
// 获取输入输出tensor
std::vector<TensorPtr> SnpeInferPriv::GetInputTensors()
{
    return input_tensors_;
}

std::vector<TensorPtr> SnpeInferPriv::GetOutputTensors()
{
    return output_tensors_;
}


// 设置输入输出tensor
int SnpeInferPriv::SetInputTensors(std::vector<TensorPtr>& tensor)
{
    return 0;
}

int SnpeInferPriv::SetOutputTensors(std::vector<TensorPtr>& tensor)
{
    return 0;
}


SnpeInferNode::SnpeInferNode()
{

}

SnpeInferNode::SnpeInferNode(const std::string& name): INode(name) 
{

}

SnpeInferNode::SnpeInferNode(const std::string& name, ModelPtr model): INode(name, model) 
{
    priv_ = std::make_shared<SnpeInferPriv>(model);
}

SnpeInferNode::~SnpeInferNode()
{

}


// 初始化
int SnpeInferNode::Init(std::string config)
{
    priv_->Init(config);

    return 0;
}

// 处理数据
int SnpeInferNode::Process(TensorPtr& input, TensorPtr& output)
{
    priv_->Process(input, output);

    return 0;
}

int SnpeInferNode::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs)
{
    priv_->Process(inputs, outputs);

    return 0;
}


// 释放资源
int SnpeInferNode::Finalize()
{
    priv_->Finalize();

    return 0;
}


// 获取输入输出节点的数量
int SnpeInferNode::GetInputSize()
{
    return priv_->GetInputSize();
}

int SnpeInferNode::GetOutputSize()
{
    return priv_->GetOutputSize();
}


// 设置输入输出节点
int SnpeInferNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs)
{
    return priv_->SetInput(inputs);
}

int SnpeInferNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs)
{
    return priv_->SetOutput(outputs);
}


// 获取输入输出节点
int SnpeInferNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs)
{
    return priv_->GetInput(inputs);
}

int SnpeInferNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs)
{
    return priv_->GetOutput(outputs);
}


// 获取参数
int SnpeInferNode::GetParam(const std::string& key, std::string& value)
{
    return priv_->GetParam(key, value);
}

// 设置参数
int SnpeInferNode::SetParam(const std::string& key, const std::string& value)
{
    return priv_->SetParam(key, value);
}


// 获取硬件IP
std::string SnpeInferNode::GetIP()
{
    return priv_->GetIP();
}

// 获取版本号
std::string SnpeInferNode::GetVersion()
{
    return priv_->GetVersion();
}


// 获取硬件IP
// 获取输入输出tensor
// std::vector<TensorPtr> SnpeInferNode::GetInputTensors()
// {
//     return priv_->GetInputTensors();
// }

// std::vector<TensorPtr> SnpeInferNode::GetOutputTensors()
// {
//     return priv_->GetOutputTensors();
// }


// 设置输入输出tensor
int SnpeInferNode::SetInputTensors(std::vector<TensorPtr>& tensor)
{
    return priv_->SetInputTensors(tensor);
}

int SnpeInferNode::SetOutputTensors(std::vector<TensorPtr>& tensor)
{
    return priv_->SetOutputTensors(tensor);
}


}