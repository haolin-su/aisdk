#include "cv_detection_yolo_postproc.h"

#include "opencv2/opencv.hpp"

#include "fibo/error.h"
#include "fibo/result.h"
#include "common/tensor_util.h"
#include "utils/postproc_opencv.h"

using namespace aisdk;
// 初始化
int YoloPostProcNode::Init(std::string config)
{
    return 0;
}

// 处理数据
int YoloPostProcNode::Process(TensorPtr& input, TensorPtr& output) 
{
    if (input == nullptr || output == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    cv::Mat input_mat, output_mat;
    if (tensor2mat(input, input_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
        
    }

    if (tensor2mat(output, output_mat) != 0) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
        
    }
    
    MetaDataCvFramePtr meta_data_ptr = std::static_pointer_cast<MetaDataCvFrame>(input->meta_data_ptr);
    // Model model = model_;

    if(YoloDetectionPostprocOpencv(input_mat, output_mat, 0.5) != ErrorCode::ERROR_CODE_OK) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
    }

    // In case the output_mat is empty, return an empty tensor
    if (output_mat.empty()) {
        meta_data_ptr->result.result_cv.detect.count = 0;
        meta_data_ptr->result.result_cv.detect.objects.clear(); 
        return ErrorCode::ERROR_CODE_OK;
    }

    meta_data_ptr->result.result_cv.detect.count = output_mat.rows;
    for (int i = 0; i < output_mat.rows; i++) {
        result::ResultCvDetectObject obj;
        obj.class_id = output_mat.at<int>(i, 0);
        obj.score = output_mat.at<float>(i, 1);
        obj.bbox.x = output_mat.at<float>(i, 2);
        obj.bbox.y = output_mat.at<float>(i, 3);
        obj.bbox.w = output_mat.at<float>(i, 4);
        obj.bbox.h = output_mat.at<float>(i, 5);
        meta_data_ptr->result.result_cv.detect.objects.push_back(obj);
    }
    return ErrorCode::ERROR_CODE_OK;
}

int YoloPostProcNode::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) 
{
    // cv::Mat input_mat = cv::Mat(inputs[0]->GetData<float>(), inputs[0]->GetShape());
    // cv::Mat output_mat;
    // YoloPreprocOpencv(input_mat, output_mat, 640, 640);
    // outputs[0]->SetData<float>(output_mat.data, output_mat.total() * output_mat.channels());
    // outputs[0]->SetShape({1, 3, 640, 640});

    return 0;
}

// 释放资源
int YoloPostProcNode::Finalize() 
{
    return 0;
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
std::vector<TensorPtr> YoloPostProcNode::GetInputTensors() 
{
    return std::vector<TensorPtr>();
}

std::vector<TensorPtr> YoloPostProcNode::GetOutputTensors() 
{
    return std::vector<TensorPtr>();
}

// 设置输入输出tensor
int YoloPostProcNode::SetInputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}

int YoloPostProcNode::SetOutputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}