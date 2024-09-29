#include "cv_segmentation_yolo_postproc.h"

#include <iostream>

#include "utils/postproc_opencv.h"
#include "common/tensor_util.h"
#include "result.h"
#include "error.h"

using namespace aisdk;
// 初始化
int YoloSegPostProcNode::Init(std::string config)
{
    std::cout << "YoloSegPostProcNode Init" << std::endl;
    // Not implemented yet
    return true;
}

// 处理数据
int YoloSegPostProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    std::cout << "YoloSegPostProcNode Process" << std::endl;

    if (input == nullptr || output == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    cv::Mat input_mat, output_mat;
    if (tensor2mat(input, input_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        
    }
    
    YoloSegmentationPostprocOpencv(input_mat, output_mat, CONFIDENCE_THRESHOLD, NUM_MASKS);

    if (mat2tensor(output_mat, output) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
    }

    // TODO: 增加直接转换成Result的代码

    return ErrorCode::ERROR_CODE_OK;
}

int YoloSegPostProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    // cv::Mat input_mat = cv::Mat(inputs[0]->GetData<float>(), inputs[0]->GetShape());
    // cv::Mat output_mat;
    // YoloPreprocOpencv(input_mat, output_mat, 640, 640);
    // outputs[0]->SetData<float>(output_mat.data, output_mat.total() * output_mat.channels());
    // outputs[0]->SetShape({1, 3, 640, 640});

    return true;
}

// 释放资源
int YoloSegPostProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int YoloSegPostProcNode::GetInputSize() 
{
    return 0;
}

int YoloSegPostProcNode::GetOutputSize() 
{
    return 0;
}

// 设置输入输出节点
int YoloSegPostProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloSegPostProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int YoloSegPostProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloSegPostProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int YoloSegPostProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}

// 设置参数
int YoloSegPostProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string YoloSegPostProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string YoloSegPostProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int YoloSegPostProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloSegPostProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int YoloSegPostProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloSegPostProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}