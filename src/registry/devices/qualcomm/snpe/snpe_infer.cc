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
    int Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output);
    int Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs);

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
    int GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor);
    int GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor);

    // 设置输入输出tensor
    int SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor);
    int SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor);
private:
    ModelPtr model_;

    std::shared_ptr<zdl::SNPE::SNPE> snpe;
};

// 初始化
int SnpeInferPriv::Init(std::string config)
{
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    bool staticQuantization = false;
    int bitWidth = 32;
    zdl::DlSystem::UserBufferMap inputMap;
    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationInputBuffers;
    createInputBufferMap(inputMap, applicationInputBuffers, snpeUserBackedInputBuffers, snpe, true, staticQuantization, bitWidth);

    return 0;
}

// 处理数据
int SnpeInferPriv::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output)
{
    // 检查input是否为空
    if (input == nullptr)
    {
        return -1;
    }

    // 检查output是否为空
    if (output == nullptr)
    {
        // TODO: 创建output
    }

    // 将input数据封装为inputMap
    zdl::DlSystem::UserBufferMap inputMap;
    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers;


    // void *input_data = nullptr;
    // if ( 0 != input->GetData(input_data)) {
    //     return -1;
    // }

    // // create SNPE user buffer from the user-backed buffer
    // zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
    // snpeUserBackedInputBuffers.push_back(ubFactory.createUserBuffer(input_data,
    //                                                             bufSize,
    //                                                             strides,
    //                                                             userBufferEncoding.get()));
    // if (snpeUserBackedBuffers.back() == nullptr)
    // {
    //     return -1;
    // }
    // // add the user-backed buffer to the inputMap, which is later on fed to the network for execution
    // inputMap.add(name, snpeUserBackedBuffers.back().get());


    // // Execute the multiple input tensorMap on the model with SNPE
    // execStatus = snpe->execute(inputMap, outputTensorMap);

    return 0;
}

int SnpeInferPriv::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs)
{
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
int SnpeInferPriv::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}

int SnpeInferPriv::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}


// 设置输入输出tensor
int SnpeInferPriv::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}

int SnpeInferPriv::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}


SnpeInfer::SnpeInfer()
{

}
SnpeInfer::SnpeInfer(const std::string& name): INode(name) 
{

}

SnpeInfer::~SnpeInfer()
{

}


// 初始化
int SnpeInfer::Init(std::string config)
{
    std::unique_ptr<zdl::SNPE::SNPE> snpe;
    bool staticQuantization = false;
    int bitWidth = 32;
    zdl::DlSystem::UserBufferMap inputMap;
    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationInputBuffers;
    createInputBufferMap(inputMap, applicationInputBuffers, snpeUserBackedInputBuffers, snpe, true, staticQuantization, bitWidth);

    return 0;
}

// 处理数据
int SnpeInfer::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output)
{

    return 0;
}

int SnpeInfer::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs)
{
    return 0;
}


// 释放资源
int SnpeInfer::Finalize()
{
    return 0;
}


// 获取输入输出节点的数量
int SnpeInfer::GetInputSize()
{
    return 0;
}

int SnpeInfer::GetOutputSize()
{
    return 0;
}


// 设置输入输出节点
int SnpeInfer::SetInput(const std::vector<std::shared_ptr<INode>>& inputs)
{
    return 0;
}

int SnpeInfer::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs)
{
    return 0;
}


// 获取输入输出节点
int SnpeInfer::GetInput(std::vector<std::shared_ptr<INode>>& inputs)
{
    return 0;
}

int SnpeInfer::GetOutput(std::vector<std::shared_ptr<INode>>& outputs)
{
    return 0;
}


// 获取参数
int SnpeInfer::GetParam(const std::string& key, std::string& value)
{
    return 0;
}

// 设置参数
int SnpeInfer::SetParam(const std::string& key, const std::string& value)
{
    return 0;
}


// 获取硬件IP
std::string SnpeInfer::GetIP()
{
    return 0;
}

// 获取版本号
std::string SnpeInfer::GetVersion()
{
    return 0;
}


// 获取硬件IP
// 获取输入输出tensor
int SnpeInfer::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}

int SnpeInfer::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}


// 设置输入输出tensor
int SnpeInfer::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}

int SnpeInfer::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor)
{
    return 0;
}


}