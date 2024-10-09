#include "cv_segmentation_yolo_preproc.h"

#include "utils/preproc_opencv.h"
#include "common/tensor_util.h"
#include "fibo/error.h"

using namespace aisdk;
// 初始化
int YoloSegPreProcNode::Init(std::string config)
{
    // Not implemented yet
    return true;
}

// 处理数据
int YoloSegPreProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{ 
    if (input == nullptr || output == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    cv::Mat input_mat, output_mat;
    if (tensor2mat(input, input_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
        
    }

    TensorShape input_shape;
    input->GetShape(input_shape);
    Layout input_layout;
    input->GetLayout(input_layout);

    if(input_layout == Layout::AISDK_LAYOUT_NHWC) {
        YoloPreprocOpencv(input_mat, output_mat, input_shape.shape[0], input_shape.shape[1]);
    } else if (input_layout == Layout::AISDK_LAYOUT_NCHW) {
        YoloPreprocOpencv(input_mat, output_mat, input_shape.shape[1], input_shape.shape[2]);
    } else {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    if (mat2tensor(output_mat, output) != 0) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
    }

    return ErrorCode::ERROR_CODE_OK;
}

int YoloSegPreProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    if (inputs.empty() || outputs.empty()) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    for (int i = 0; i < inputs.size(); i++)
    {
        auto &input = inputs[i];
        auto &output = outputs[i];

        cv::Mat input_mat, output_mat;
        tensor2mat(input, input_mat);
        
        TensorShape input_shape;
        input->GetShape(input_shape);
        Layout input_layout;
        input->GetLayout(input_layout);

        if(input_layout == Layout::AISDK_LAYOUT_NHWC) {
            YoloPreprocOpencv(input_mat, output_mat, input_shape.shape[0], input_shape.shape[1]);
        } else if (input_layout == Layout::AISDK_LAYOUT_NCHW) {
            YoloPreprocOpencv(input_mat, output_mat, input_shape.shape[1], input_shape.shape[2]);
        } else {
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

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
std::vector<TensorPtr> YoloSegPreProcNode::GetInputTensors() 
{
    return std::vector<TensorPtr>();
}

std::vector<TensorPtr> YoloSegPreProcNode::GetOutputTensors() 
{
    return std::vector<TensorPtr>();
}

// 设置输入输出tensor
int YoloSegPreProcNode::SetInputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}

int YoloSegPreProcNode::SetOutputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0; 
}