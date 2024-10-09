#include "cv_segmentation_yolo_postproc.h"

#include <iostream>

#include "utils/postproc_opencv.h"
#include "common/tensor_util.h"
#include "fibo/result.h"
#include "fibo/error.h"

using namespace aisdk;
// 初始化
int YoloSegPostProcNode::Init(std::string config)
{
    // Not implemented yet
    return true;
}

// 处理数据
int YoloSegPostProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{ 
    // segmentation have two output nodes, use the multiple output node process function

//     std::cout << "YoloSegmentationPostProcNode Process" << std::endl;

//     if (input == nullptr || output == nullptr) {
//         return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
//     }

//     cv::Mat input_mat, output_mat;
//     if (tensor2mat(input, input_mat) != 0) {
//         return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
        
//     }
    
//     if(YoloSegmentationPostprocOpencv(input_mat, output_mat, CONFIDENCE_THRESHOLD, NUM_MASKS) != ErrorCode::ERROR_CODE_OK) {
//         return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
//     }

//     // TODO：以下需要根据segmentation的结果进行修改
//     // In case the output_mat is empty, return an empty tensor
//     if (output_mat.empty()) {
//         output->meta_data_ptr->result_.result_cv_.detect.count = 0;
//         output->meta_data_ptr->result_.result_cv_.detect.objects.clear(); 
//         return ErrorCode::ERROR_CODE_OK;
//     }

//     // 待定：返回tensor或者直接转换成Result的格式
// # if 0
//     if (mat2tensor(output_mat, output) != 0) {
//         return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
//     }
// #else 
//     output->meta_data_ptr->result_.result_cv_.detect.count = output_mat.rows;
//     for (int i = 0; i < output_mat.rows; i++) {
//         result::ResultCvDetectObject obj;
//         obj.class_id = output_mat.at<int>(i, 0);
//         obj.score = output_mat.at<float>(i, 1);
//         obj.bbox.x = output_mat.at<float>(i, 2);
//         obj.bbox.y = output_mat.at<float>(i, 3);
//         obj.bbox.w = output_mat.at<float>(i, 4);
//         obj.bbox.h = output_mat.at<float>(i, 5);
//         output->meta_data_ptr->result_.result_cv_.detect.objects.push_back(obj);
//     }
// #endif
//     std::cout << "YoloSegPostProcNode Process Done" << std::endl;

//     return ErrorCode::ERROR_CODE_OK;
    return 0;
}

int YoloSegPostProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    if (inputs.size() != 2 || outputs.size() != 2 || 
        inputs[0] == nullptr || inputs[1] == nullptr || 
        outputs[0] == nullptr || outputs[1] == nullptr) {
        return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
    }

    // TODO：后续看如何获取数据
    cv::Mat prediction_input, mask_input;
    cv::Mat prediction_output, mask_output;
    tensor2mat(inputs[0], prediction_input);
    tensor2mat(inputs[1], mask_input);
    tensor2mat(outputs[0], prediction_output);
    tensor2mat(outputs[1], mask_output);

    std::vector<int> tensor_shape_0{1, 116, 8400};
    std::vector<int> tensor_shape_1{1, 32, 160, 160};
    float* data = nullptr;
    inputs[0]->GetData(data);
    prediction_input = cv::Mat(3, tensor_shape_0.data(), CV_32F, data).clone();

    inputs[1]->GetData(data);
    mask_input = cv::Mat(4, tensor_shape_1.data(), CV_32F, data).clone();

    // use the user defined arguments instead of the default values for CONFIDENCE_THRESHOLD and NUM_MASKS
    if(YoloSegmentationPostprocOpencv(prediction_input, mask_input, prediction_output, mask_output, CONFIDENCE_THRESHOLD, NUM_MASKS) != ErrorCode::ERROR_CODE_OK) {
        return ErrorCode::ERROR_CODE_CV_INVALID_RESULT;
    }

    // TODO：以下需要根据segmentation的结果进行修改
    // In case the output_mat is empty, return an empty tensor
#if 0
    if (prediction_output.empty() || mask_output.empty()) {
        outputs[0]->meta_data_ptr->result.result_cv.detect.count = 0;
        outputs[0]->meta_data_ptr->result.result_cv.detect.objects.clear(); 
        outputs[1]->meta_data_ptr->result.result_cv.segment.count = 0;
        outputs[1]->meta_data_ptr->result.result_cv.segment.objects.clear(); 
        return ErrorCode::ERROR_CODE_OK;
    }

    // 待定：返回tensor或者直接转换成Result的格式
    outputs[0]->meta_data_ptr->result.result_cv.detect.count = prediction_output.rows;
    for (int i = 0; i < prediction_output.rows; i++) {
        result::ResultCvDetectObject obj;
        obj.class_id = prediction_output.at<int>(i, 0);
        obj.score = prediction_output.at<float>(i, 1);
        obj.bbox.x = prediction_output.at<float>(i, 2);
        obj.bbox.y = prediction_output.at<float>(i, 3);
        obj.bbox.w = prediction_output.at<float>(i, 4);
        obj.bbox.h = prediction_output.at<float>(i, 5);
        outputs[0]->meta_data_ptr->result.result_cv.detect.objects.push_back(obj);
    }

    outputs[1]->meta_data_ptr->result.result_cv.segment.count = mask_output.rows;
    for (int i = 0; i < mask_output.rows; i++) {
        outputs[1]->meta_data_ptr->result.result_cv.segment.objects[i].planner_num = i;

        result::SegPlanner obj;
        obj.height = mask_output.at<int>(i, 1);
        obj.width = mask_output.at<int>(i, 2);
        obj.size = mask_output.at<int>(i, 0) * obj.height * obj.width;

	    for (int i = 0;i < obj.height;i++)
	    {
	    	for (int j = 0;j < obj.width;j++)
	    	{
	    		for (int k = 0;k < 3;k++)
	    		{
	    			obj.data[i * obj.width * 3 + j * 3 + k] = mask_output.row(i).at<cv::Vec3b>(i, j)[k];
	    		}
	    	}
	    }

            outputs[1]->meta_data_ptr->result.result_cv.segment.objects[i].planners.push_back(obj);
        }
#endif
    return ErrorCode::ERROR_CODE_OK;
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
std::vector<TensorPtr> YoloSegPostProcNode::GetInputTensors() 
{
    return std::vector<TensorPtr>();
}

std::vector<TensorPtr> YoloSegPostProcNode::GetOutputTensors() 
{
    return std::vector<TensorPtr>();
}

// 设置输入输出tensor
int YoloSegPostProcNode::SetInputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}

int YoloSegPostProcNode::SetOutputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}