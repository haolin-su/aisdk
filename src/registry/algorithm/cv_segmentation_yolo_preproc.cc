#include "cv_segmentation_yolo_preproc.h"
#include "utils/preproc_opencv.h"
#include "common/tensor_util.h"

using namespace aisdk;
// 初始化
int YoloSegPreProcNode::Init(std::string config)
{
    return true;
}

// 处理数据
int YoloSegPreProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    cv::Mat input_mat, output_mat;
    tensor2mat(input, input_mat);
    
    YoloPreprocOpencv(input_mat, output_mat, 480, 640);

    mat2tensor(output_mat, output);

    return true;
}

int YoloSegPreProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    for (int i = 0; i < inputs.size(); i++)
    {
        auto &input = inputs[i];
        auto &output = outputs[i];

        cv::Mat input_mat, output_mat;
        tensor2mat(input, input_mat);
        
        YoloPreprocOpencv(input_mat, output_mat, 640, 640);

        mat2tensor(output_mat, output);
    }

    return true;
}

// 释放资源
int YoloSegPreProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int YoloSegPreProcNode::GetInputSize() 
{
    return 0;
}

int YoloSegPreProcNode::GetOutputSize() 
{
    return 0;
}

// 设置输入输出节点
int YoloSegPreProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloSegPreProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int YoloSegPreProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloSegPreProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int YoloSegPreProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}

// 设置参数
int YoloSegPreProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string YoloSegPreProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string YoloSegPreProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int YoloSegPreProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloSegPreProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int YoloSegPreProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloSegPreProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0; 
}