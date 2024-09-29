#include "cv_detection_yolo_postproc.h"

#include <iostream>

#include "error.h"
#include "utils/postproc_opencv.h"
#include "common/tensor_util.h"
#include "core/tensor/tensor.h"
#include "result.h"

using namespace aisdk;
// 初始化
int YoloPostProcNode::Init(std::string config)
{
    std::cout << "YoloPostProcNode Init" << std::endl;
    // Not implemented yet
    return true;
}

// 处理数据
int YoloPostProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    std::cout << "YoloDectionPostProcNode Process" << std::endl;

    if (input == nullptr || output == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    cv::Mat input_mat, output_mat;
    if (tensor2mat(input, input_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        
    }
    
    if(YoloDetectionPostprocOpencv(input_mat, output_mat, CONFIDENCE_THRESHOLD) != ErrorCode::ERROR_CODE_OK) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
    }

    // In case the output_mat is empty, return an empty tensor
    if (output_mat.empty()) {
        output->meta_data_ptr_->result_.result_cv_.detect.count = 0;
        output->meta_data_ptr_->result_.result_cv_.detect.objects.clear(); 
        return ErrorCode::ERROR_CODE_OK;
    }

    // 待定：返回tensor或者直接转换成Result的格式
# if 0
    if (mat2tensor(output_mat, output) != 0) {
        return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
    }
#else 
    output->meta_data_ptr_->result_.result_cv_.detect.count = output_mat.rows;
    for (int i = 0; i < output_mat.rows; i++) {
        result::ResultCvDetectObject obj;
        obj.class_id = output_mat.at<int>(i, 0);
        obj.score = output_mat.at<float>(i, 1);
        obj.bbox.x = output_mat.at<float>(i, 2);
        obj.bbox.y = output_mat.at<float>(i, 3);
        obj.bbox.w = output_mat.at<float>(i, 4);
        obj.bbox.h = output_mat.at<float>(i, 5);
        output->meta_data_ptr_->result_.result_cv_.detect.objects.push_back(obj);
    }
#endif
    std::cout << "YoloSegPostProcNode Process Done" << std::endl;

    return ErrorCode::ERROR_CODE_OK;
}

int YoloPostProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    // cv::Mat input_mat = cv::Mat(inputs[0]->GetData<float>(), inputs[0]->GetShape());
    // cv::Mat output_mat;
    // YoloPreprocOpencv(input_mat, output_mat, 640, 640);
    // outputs[0]->SetData<float>(output_mat.data, output_mat.total() * output_mat.channels());
    // outputs[0]->SetShape({1, 3, 640, 640});

    return true;
}

// 释放资源
int YoloPostProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int YoloPostProcNode::GetInputSize() 
{
    return 0;
}

int YoloPostProcNode::GetOutputSize() 
{
    return 0;
}

// 设置输入输出节点
int YoloPostProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPostProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int YoloPostProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPostProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int YoloPostProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}

// 设置参数
int YoloPostProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string YoloPostProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string YoloPostProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int YoloPostProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPostProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int YoloPostProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPostProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}