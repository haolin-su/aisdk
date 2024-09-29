#include "cv_detection_yolo_preproc.h"
#include "utils/preproc_opencv.h"
#include "common/tensor_util.h"
#include "error.h"

using namespace aisdk;
// 初始化
int YoloPreProcNode::Init(std::string config)
{
    // Not implemented yet
    return true;
}

// 处理数据
int YoloPreProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    if (input == nullptr || output == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    cv::Mat input_mat, output_mat;
    if (tensor2mat(input, input_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        
    }
    
    YoloPreprocOpencv(input_mat, output_mat, 480, 640);

    if (mat2tensor(output_mat, output) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
    }

    return ErrorCode::ERROR_CODE_OK;
}

int YoloPreProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
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
int YoloPreProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPreProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int YoloPreProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPreProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0; 
}