#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iterator>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

// #include "DlSystem/UserBufferMap.hpp"
// #include "DlSystem/IUserBuffer.hpp"
// #include "DlSystem/DlEnums.hpp"
// #include "DlSystem/RuntimeList.hpp"
// #include "DlContainer/IDlContainer.hpp"

// #include "SNPE/SNPE.hpp"
// #include "SNPE/SNPEBuilder.hpp"
// #include "SNPE/SNPEFactory.hpp"

#include "registry/algorithm/preproc_node/utils/preproc_opencv.h"
#include "registry/algorithm/preproc_node/utils/preproc_opencv.cc"
#include "registry/algorithm/postproc_node/utils/postproc_opencv.h"
#include "registry/algorithm/postproc_node/utils/postproc_opencv.cc"
#include "fibo/result.h"
#include "fibo/error.h"

void CreateMatFromRaw(std::string &file_path, std::vector<int> &input_shape, cv::Mat &mat)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open())
    {
        std::cerr << "Error opening " << file_path << std::endl;
        return;
    }

    size_t total_size = 1 * sizeof(float);
    int count = 0;
    for (auto i : input_shape)
    {
        total_size *= i;
        count++;
    }

    std::vector<float> buffer(total_size / sizeof(float));
    file.read(reinterpret_cast<char *>(buffer.data()), total_size);

    if (!file)
    {
        std::cerr << "Error reading from " << file_path << std::endl;
        return;
    }

    file.close();

    mat = cv::Mat(count, input_shape.data(), CV_32F, buffer.data()).clone();
}

void SaveMatAsRaw(const cv::Mat& mat, const std::string& filename) {
    if (!mat.isContinuous()) {
        std::cerr << "Matrix is not continuous." << std::endl;
        return;
    }

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Could not open the file!" << std::endl;
        return;
    }

    file.write(reinterpret_cast<const char*>(mat.data), mat.total() * mat.elemSize());

    file.close();
}

bool check_result(int32_t error_code, aisdk::ErrorCode expected_error_code)
{
    bool ret = false;
    if (error_code != expected_error_code)
    {
        std::cerr << "Test Failed: Expected " << expected_error_code << " but got " << error_code << std::endl;
    }
    else
    {
        std::cout << "Test Passed" << std::endl;
        ret = true;
    }
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    return ret;
}

bool TEST_1()
{
    std::cout << __func__ << std::endl;
    std::string file_path = "outputs_0.raw";
    std::vector<int> input_shape = {1, 116, 8400};
    cv::Mat mat(3, input_shape.data(), CV_32F);

    CreateMatFromRaw(file_path, input_shape, mat);

    cv::Mat out_mat;

    int ret = aisdk::YoloDetectionPostprocOpencv(mat, out_mat, 0.8);
    std::cout << "ret: " << ret << std::endl;
}

// 测试总流程,正确输入
// int32_t YoloSegmentationPostprocOpencv(const cv::Mat& prediction_input, const cv::Mat& mask_input, 
//                                          cv::Mat& prediction_output, cv::Mat& mask_output, float confidence_threshold, int32_t num_masks);
bool TEST_2_0()
{
    bool ret = false;

    std::string file_path_1 = "seg_input_1.raw";
    std::string file_path_2 = "seg_input_2.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);
    CreateMatFromRaw(file_path_2, input_shape_2, mat_2);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.5;
    int num_masks = 32;

    if(check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::string gt_file_path_1 = "prediction_output.raw";
        std::string gt_file_path_2 = "mask_output.raw";

        cv::Mat gt_out_mat_1;
        cv::Mat gt_out_mat_2;

        std::vector<int> gt_out_shape_1;
        std::vector<int> gt_out_shape_2;

        for (int i = 0; i < out_mat_1.dims; ++i) {
            gt_out_shape_1.push_back(out_mat_1.size[i]);
        }

        for (int i = 0; i < out_mat_2.dims; ++i) {
            gt_out_shape_2.push_back(out_mat_2.size[i]);
        }

        CreateMatFromRaw(gt_file_path_1, gt_out_shape_1, gt_out_mat_1);
        CreateMatFromRaw(gt_file_path_2, gt_out_shape_2, gt_out_mat_2);
        
        cv::Mat diff_1;
        cv::Mat diff_2;
        cv::compare(out_mat_1, gt_out_mat_1, diff_1, cv::CMP_NE);
        cv::compare(out_mat_2, gt_out_mat_2, diff_2, cv::CMP_NE);

        if(cv::countNonZero(diff_1) == 0 && cv::countNonZero(diff_2) == 0){
            ret = true;
        }
    }

    return ret;
}

// 测试总流程，输入非法参数 mat_1为空
bool TEST_2_1()
{
    cv::Mat mat_1;
    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.5;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试总流程，输入非法参数 mat_2为空
bool TEST_2_2()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    cv::Mat mat_2;

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.5;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试总流程，输入非法参数 confidence_threshold小于0
bool TEST_2_3()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = -0.1;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试总流程，输入非法参数 confidence_threshold大于1
bool TEST_2_4()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 1.1;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试总流程，输入非法参数 num_masks小于0
bool TEST_2_5()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.5;
    int num_masks = -1;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试总流程，输入临界值 confidence_threshold=0
bool TEST_2_6()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.0;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试总流程，输入临界值 confidence_threshold=1
bool TEST_2_7()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 1.0;
    int num_masks = 32;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试总流程，输入临界值 num_masks=0
bool TEST_2_8()
{
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    float confidence_threshold = 0.5;
    int num_masks = 0;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, confidence_threshold, num_masks), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试Segmentation的第一个output后处理，正确输入
// int32_t ProcessSegmentationPredictions(const cv::Mat& results, float confidence_threshold, int32_t num_masks, Predictions& predictions, std::vector<cv::Mat>& mask_predictions)
bool TEST_3_0()
{
    bool ret = false;

    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 0.5;
    int num_masks = 32;

    if(check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_OK)){

        ret = true;
    }
    return ret;
}

// 测试Segmentation的第一个output后处理，输入非法参数 mat_1为空
bool TEST_3_1()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    cv::Mat mat_1;

    float confidence_threshold = 0.5;
    int num_masks = 32;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试Segmentation的第一个output后处理，输入非法参数 confidence_threshold小于0
bool TEST_3_2()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = -0.1;
    int num_masks = 32;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试Segmentation的第一个output后处理，输入非法参数 confidence_threshold大于1
bool TEST_3_3()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 1.1;
    int num_masks = 32;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试Segmentation的第一个output后处理，输入非法参数 num_masks小于0
bool TEST_3_4()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 0.5;
    int num_masks = -1;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试Segmentation的第一个output后处理，输入临界值 confidence_threshold=0
bool TEST_3_5()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 0.0;
    int num_masks = 32;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试Segmentation的第一个output后处理，输入临界值 confidence_threshold=1
bool TEST_3_6()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 1.0;
    int num_masks = 32;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试Segmentation的第一个output后处理，输入临界值 num_masks=0
bool TEST_3_7()
{
    aisdk::Predictions predictions;
    std::vector<cv::Mat> mask_predictions;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    float confidence_threshold = 1.0;
    int num_masks = 0;

    return check_result(aisdk::ProcessSegmentationPredictions(mat_1, confidence_threshold, num_masks, predictions, mask_predictions), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试bbox的抽取函数, 正确输入
// int32_t ExtractBox(const cv::Mat& output, std::vector<int32_t>& box);
bool TEST_4_0()
{
    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    cv::Mat output = aisdk::Squeeze(mat_1);
    cv::transpose(output, output);

    std::vector<int32_t> box;

    return check_result(aisdk::ExtractBox(output, box), aisdk::ErrorCode::ERROR_CODE_OK);
}

// 测试bbox的抽取函数, 输入非法参数 output为空
bool TEST_4_1()
{
    cv::Mat output;
    std::vector<int32_t> box;

    return check_result(aisdk::ExtractBox(output, box), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试bbox的抽取函数, 正确输入
// int32_t ExtractBox(const cv::Mat& output, std::vector<int32_t>& box);
bool TEST_5_0()
{
    bool ret = false;

    std::string file_path_1 = "seg_input_1.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    CreateMatFromRaw(file_path_1, input_shape_1, mat_1);

    cv::Mat output = aisdk::Squeeze(mat_1);
    cv::transpose(output, output);

    std::vector<int32_t> box {0, 0, 0, 0};

    if(check_result(aisdk::ExtractBox(output, box), aisdk::ErrorCode::ERROR_CODE_OK)){
        for(auto i : box){
            std::cout << i << " ";
        }
        std::cout << std::endl;
        ret = true;
    }
    return ret;
}

// 测试bbox的抽取函数, 输入非法参数 output为空
bool TEST_5_1()
{
    cv::Mat output;

    std::vector<int32_t> box {0, 0, 0, 0};

    return check_result(aisdk::ExtractBox(output, box), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 正确输入
// int32_t NMSBoxes(std::vector<std::vector<int32_t>>& boxes, const std::vector<float>& scores, 
//                    const float confidence_threshold, const float iou_threshold, std::vector<int32_t> &indices)
bool TEST_6_0()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::vector<int32_t> expected_indices {0, 1, 2};
        if(indices == expected_indices){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 正确输入, boxes为空
bool TEST_6_1()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes;
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        if(indices.empty()){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 非法输入 boxes.size() != scores.size()
bool TEST_6_2()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    return check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 非法输入 confidence_threshold < 0
bool TEST_6_3()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = -0.1;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    return check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 非法输入 confidence_threshold > 1
bool TEST_6_4()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 1.1;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    return check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 非法输入 iou_threshold < 0
bool TEST_6_5()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = -0.1;
    std::vector<int32_t> indices;

    return check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 非法输入 iou_threshold > 1
bool TEST_6_6()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 1.1;
    std::vector<int32_t> indices;

    return check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_CV_INVALID_PARAM);
}

// 测试NMS, 正确输入，临界值confidence_threshold=0
bool TEST_6_7()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::vector<int32_t> expected_indices {0, 1, 2};
        if(indices == expected_indices){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 正确输入，临界值confidence_threshold=1
bool TEST_6_8()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 1.0;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        if(indices.empty()){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 正确输入，临界值iou_threshold=0
bool TEST_6_9()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 0;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::vector<int32_t> expected_indices {0, 1, 2};
        if(indices == expected_indices){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 正确输入，临界值iou_threshold=1
bool TEST_6_10()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.7};
    float confidence_threshold = 0.5;
    float iou_threshold = 1.0;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::vector<int32_t> expected_indices {0, 1, 2};
        if(indices == expected_indices){
            ret = true;
        }
    }
    return ret;
}

// 测试NMS, 正确输入, 有不符合threshold的box
bool TEST_6_11()
{
    bool ret = false;

    std::vector<std::vector<int32_t>> boxes {{0, 0, 0, 0}, {1, 1, 1, 1}, {2, 2, 2, 2}};
    std::vector<float> scores {0.9, 0.8, 0.3};
    float confidence_threshold = 0.5;
    float iou_threshold = 0.5;
    std::vector<int32_t> indices;

    if(check_result(aisdk::NMSBoxes(boxes, scores, confidence_threshold, iou_threshold, indices), aisdk::ErrorCode::ERROR_CODE_OK)){
        std::vector<int32_t> expected_indices {0, 1};
        if(indices == expected_indices){
            ret = true;
        }
    }
    return ret;
}


int main(int argc, char *argv[])
{

    int total_count = 0;
    int pass_count = 0;
    int fail_count = 0;

    std::vector<std::string> test_funcs = {"TEST_2_0", "TEST_2_1", "TEST_2_2", "TEST_2_3", "TEST_2_4", "TEST_2_5", "TEST_2_6", "TEST_2_7", "TEST_2_8",
                                            "TEST_3_0", "TEST_3_1", "TEST_3_2", "TEST_3_3", "TEST_3_4", "TEST_3_5", "TEST_3_6", "TEST_3_7",
                                            "TEST_4_0", "TEST_4_1",
                                            "TEST_5_0", "TEST_5_1",
                                            "TEST_6_0", "TEST_6_1", "TEST_6_2", "TEST_6_3", "TEST_6_4", "TEST_6_5", "TEST_6_6", "TEST_6_7", "TEST_6_8", "TEST_6_9", "TEST_6_10", "TEST_6_11"};
    std::map<std::string, std::function<bool()>> test_map = {
        {"TEST_2_0", TEST_2_0},
        {"TEST_2_1", TEST_2_1},
        {"TEST_2_2", TEST_2_2},
        {"TEST_2_3", TEST_2_3},
        {"TEST_2_4", TEST_2_4},
        {"TEST_2_5", TEST_2_5},
        {"TEST_2_6", TEST_2_6},
        {"TEST_2_7", TEST_2_7},
        {"TEST_2_8", TEST_2_8},
        {"TEST_3_0", TEST_3_0},
        {"TEST_3_1", TEST_3_1},
        {"TEST_3_2", TEST_3_2},
        {"TEST_3_3", TEST_3_3},
        {"TEST_3_4", TEST_3_4},
        {"TEST_3_5", TEST_3_5},
        {"TEST_3_6", TEST_3_6},
        {"TEST_3_7", TEST_3_7},
        {"TEST_4_0", TEST_4_0},
        {"TEST_4_1", TEST_4_1},
        {"TEST_5_0", TEST_5_0},
        {"TEST_5_1", TEST_5_1},
        {"TEST_6_0", TEST_6_0},
        {"TEST_6_1", TEST_6_1},
        {"TEST_6_2", TEST_6_2},
        {"TEST_6_3", TEST_6_3},
        {"TEST_6_4", TEST_6_4},
        {"TEST_6_5", TEST_6_5},
        {"TEST_6_6", TEST_6_6},
        {"TEST_6_7", TEST_6_7},
        {"TEST_6_8", TEST_6_8},
        {"TEST_6_9", TEST_6_9},
        {"TEST_6_10", TEST_6_10},
        {"TEST_6_11", TEST_6_11}
    };
    std::vector<std::string> failed_tests;

    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "Test Begin" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    for (const auto &func_name : test_funcs)
    {
        std::cout << "Running " << func_name << "..." << std::endl;
        auto it = test_map.find(func_name);
        if (it != test_map.end())
        {

            if (it->second() == true)
            {
                pass_count++;
            }
            else
            {
                fail_count++;
                failed_tests.push_back(func_name);
            }
            total_count++;
        }
    }
    std::cout << "Total count: " << total_count << ", Pass count: " << pass_count << ", Fail count: " << fail_count << std::endl;
    if (fail_count > 0)
    {
        std::cout << "Failed tests: " << std::endl;
        for (const auto &failed_test : failed_tests)
        {
            std::cout << failed_test << std::endl;
        }
    }

    return 0;
}