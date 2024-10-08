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

#include "DlSystem/UserBufferMap.hpp"
#include "DlSystem/IUserBuffer.hpp"
#include "DlSystem/DlEnums.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "DlContainer/IDlContainer.hpp"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEBuilder.hpp"
#include "SNPE/SNPEFactory.hpp"

#include "registry/algorithm/utils/preproc_opencv.h"
#include "registry/algorithm/utils/preproc_opencv.cc"
#include "registry/algorithm/utils/postproc_opencv.h"
#include "registry/algorithm/utils/postproc_opencv.cc"
#include "result.h"
#include "error.h"

void create_mat_from_raw(std::string& file_path, std::vector<int>&input_shape, cv::Mat& mat)
{
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening " << file_path << std::endl;
        return;
    }

    size_t total_size = 1 * sizeof(float);
    int count = 0;
    for (auto i : input_shape) {
        total_size *= i;
        count++;
    }

    std::vector<float> buffer(total_size / sizeof(float));
    file.read(reinterpret_cast<char*>(buffer.data()), total_size);

    if (!file) {
        std::cerr << "Error reading from " << file_path << std::endl;
        return;
    }

    file.close();

    mat = cv::Mat(count, input_shape.data(), CV_32F, buffer.data()).clone();
}

bool check_result(int32_t error_code, aisdk::ErrorCode expected_error_code)
{
    bool ret = false;
    if (error_code != expected_error_code) {
        std::cerr << "Test Failed: Expected " << expected_error_code << " but got " << error_code << std::endl;
    } else {
        std::cout << "Test Passed" << std::endl;
        ret = true;
    }
    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    return ret;
}

bool TEST_1(){
    std::cout << __func__ << std::endl;
    std::string file_path = "outputs_0.raw";
    std::vector<int> input_shape = {1, 116, 8400};
    cv::Mat mat(3, input_shape.data(), CV_32F);

    create_mat_from_raw(file_path, input_shape, mat);

    cv::Mat out_mat;

    int ret = aisdk::YoloDetectionPostprocOpencv(mat, out_mat, 0.8);
    std::cout << "ret: " << ret << std::endl;
}

// 测试总流程,正确输入
bool TEST_2_0(){
    std::cout << __func__ << std::endl;
    std::string file_path_1 = "seg_input_1.raw";
    std::string file_path_2 = "seg_input_2.raw";
    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    create_mat_from_raw(file_path_1, input_shape_1, mat_1);
    create_mat_from_raw(file_path_2, input_shape_2, mat_2);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, 0.5, 32), aisdk::ErrorCode::ERROR_CODE_OK);
}

//
bool TEST_2_1(){
    std::cout << __func__ << std::endl;

    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);
    cv::Mat mat_2;

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, 0.5, 32), aisdk::ErrorCode::ERROR_CODE_OK);
}

bool TEST_2_2(){
    std::cout << __func__ << std::endl;

    cv::Mat mat_1;
    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, 0.5, 32), aisdk::ErrorCode::ERROR_CODE_OK);
}

bool TEST_2_3(){
    std::cout << __func__ << std::endl;

    std::vector<int> input_shape_1 = {1, 116, 8400};
    cv::Mat mat_1(3, input_shape_1.data(), CV_32F);

    std::vector<int> input_shape_2 = {1, 32, 160, 160};
    cv::Mat mat_2(4, input_shape_2.data(), CV_32F);

    cv::Mat out_mat_1;
    cv::Mat out_mat_2;

    return check_result(aisdk::YoloSegmentationPostprocOpencv(mat_1, mat_2, out_mat_1, out_mat_2, 0, 32), aisdk::ErrorCode::ERROR_CODE_OK);
}

int main(int argc, char* argv[]) {

    int pass_count = 0;
    int fail_count = 0;

    // std::vector<std::string> test_funcs = {"TEST_1", "TEST_2_0", "TEST_2_1"};
    // std::map<std::string, std::function<bool()>> test_map = {
    //     {"TEST_1", TEST_1}, 
    //     {"TEST_2_0", TEST_2_0},
    // };
    std::vector<std::string> test_funcs = {"TEST_2_0", "TEST_2_1"};
    std::map<std::string, std::function<bool()>> test_map = {
        {"TEST_2_0", TEST_2_0}, 
        {"TEST_2_1", TEST_2_1}
    };
    std::vector<std::string> failed_tests;

    std::cout << "-------------------------------------------------------------------------------" << std::endl;
    std::cout << "Test Begin" << std::endl;
    std::cout << "-------------------------------------------------------------------------------" << std::endl;

    for (const auto& func_name : test_funcs) {
        std::cout << "Running " << func_name << "..." << std::endl;
        auto it = test_map.find(func_name);
        if (it != test_map.end()) {

                if (it->second() == true) {
                    pass_count++;
                } else {
                    fail_count++;
                    failed_tests.push_back(func_name);
                }
        }
    }
    std::cout << "Pass count: " << pass_count << ", Fail count: " << fail_count << std::endl;
    std::cout << "Failed tests: " << std::endl;
    for (const auto& failed_test : failed_tests) {
        std::cout << failed_test << std::endl;
    }
    return 0;
}