#include "api/api_cv.h"

#include <iostream>
#include <string>
#include <opencv2/opencv.hpp>

#include "command_parser/command_parser.h"

int ParseAndCheckArgs(int argc, const char *argv[], CommandParser &options)
{
    options.AddOption("--model_file", "", "Model file path");
    options.AddOption("--input_file", "", "Input file path");
    options.AddOption("--output_file", "", "Output file path");
    options.AddOption("--runtime", "CPU", "Runtime mode (CPU/GPU)");

    options.ParseArgs(argc, argv);

    return 0;
}

int main(int argc, const char* argv[]) {
    // 解析参数
    CommandParser options;
    // Parse and check arguments
    int ret = ParseAndCheckArgs(argc, argv, options);
    if (ret != 0) {
        printf("ParseAndCheckArgs failed\n");
        return ret;
    }

    // std::string model_tmp = options.GetqStringOption("--model");
    std::string model_file = options.GetStringOption("--model_file");
    std::string input_file = options.GetStringOption("--input_file");
    std::string output_file = options.GetStringOption("--output_file");
    std::string runtime = options.GetStringOption("--runtime");

    printf("model_file: %s\n", model_file.c_str());
    printf("input_file: %s\n", input_file.c_str());
    printf("output_file: %s\n", output_file.c_str());
    printf("runtime: %s\n", runtime.c_str());

    aisdk::CVAPI cvapi;

    // 初始化
    cvapi.Init(model_file, "0");

    // 推理
    // aisdk::result::cv::ResultCv result;
    // ret = cvapi.InferSync(input_file, result);
    // if (ret != 0) {
    //     printf("InferSync failed\n");
    //     return ret;
    // }

    // 释放
    cvapi.Release();

    return 0;
}