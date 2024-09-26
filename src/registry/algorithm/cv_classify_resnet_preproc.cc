#include "cv_classify_resnet_preproc.h"
#include "utils/preproc_opencv.h"
#include "common/tensor_util.h"

using namespace aisdk;
// 初始化
int ResnetPreProcNode::Init(std::string config)
{
    // TODO: 解析配置

    return true;
}

// 处理数据
int ResnetPreProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    if (input == nullptr) 
    {
        return -1;
    }

    cv::Mat input_mat, output_mat;
    tensor2mat(input, input_mat);
    
    ResnetPreprocOpencv(input_mat, output_mat, 640, 640);

    mat2tensor(output_mat, output);

    return true;
}
int ResnetPreProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    for (int i = 0; i < inputs.size(); i++)
    {
        auto &input = inputs[i];
        auto &output = outputs[i];

        cv::Mat input_mat, output_mat;
        tensor2mat(input, input_mat);
        
        ResnetPreprocOpencv(input_mat, output_mat, 640, 640);

        mat2tensor(output_mat, output);
    }
    return true;
}

// 释放资源
int ResnetPreProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int ResnetPreProcNode::GetInputSize() 
{
    return 1;
}
int ResnetPreProcNode::GetOutputSize() 
{
    return 1;
}

// 设置输入输出节点
int ResnetPreProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}
int ResnetPreProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int ResnetPreProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}
int ResnetPreProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int ResnetPreProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}
// 设置参数
int ResnetPreProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string ResnetPreProcNode::GetIP() 
{ 
    return ""; 
}
// 获取版本号
std::string ResnetPreProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int ResnetPreProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}
int ResnetPreProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int ResnetPreProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}
int ResnetPreProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}