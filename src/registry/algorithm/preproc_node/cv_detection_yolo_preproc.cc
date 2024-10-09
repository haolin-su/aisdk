#include "cv_detection_yolo_preproc.h"
#include "utils/preproc_opencv.h"
#include "common/tensor_util.h"

using namespace aisdk;
// 初始化
int YoloPreProcNode::Init(std::string config)
{
    return true;
}

// 处理数据
int YoloPreProcNode::Process(TensorPtr& input, TensorPtr& output) 
{
    cv::Mat input_mat, output_mat;
    tensor2mat(input, input_mat);
    
    YoloPreprocOpencv(input_mat, output_mat, 640, 640);

    mat2tensor(output_mat, output);

    return true;
}

int YoloPreProcNode::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) 
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
int YoloPreProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int YoloPreProcNode::GetInputSize() 
{
    return 0;
}

int YoloPreProcNode::GetOutputSize() 
{
    return 0;
}

// 设置输入输出节点
int YoloPreProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPreProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int YoloPreProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPreProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int YoloPreProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}

// 设置参数
int YoloPreProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string YoloPreProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string YoloPreProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
std::vector<TensorPtr> YoloPreProcNode::GetInputTensors() 
{
    return std::vector<TensorPtr>();
}

std::vector<TensorPtr> YoloPreProcNode::GetOutputTensors() 
{
    return std::vector<TensorPtr>();
}

// 设置输入输出tensor
int YoloPreProcNode::SetInputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}

int YoloPreProcNode::SetOutputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0; 
}