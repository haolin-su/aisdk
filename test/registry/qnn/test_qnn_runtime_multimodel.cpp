#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <vector>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <iterator>
#include <algorithm>

#include "QnnInterface.h"
#include "QnnContext.h"
#include "QnnGraph.h"
#include "QnnTensor.h"
#include "QnnTypes.h"
#include "System/QnnSystemInterface.h"

#include "qnn_util.hpp"
#include "command_parser/command_parser.h"

int ParseAndCheckArgs(int argc, const char *argv[], CommandParser &options)
{
    options.AddOption("--backend_file", "libQnnGpu.so", "QNN backend file path");
    options.AddOption("--qnn_modeldlc_file", "libQnnModelDlc.so", "QNN model dlc file path");
    options.AddOption("--qnn_model_file", "", "QNN model file path");
    options.AddOption("--input_file", "", "Input file path");
    options.AddOption("--output_file", "", "Output file path");
    options.AddOption("--runtime", "CPU", "Runtime mode (CPU/GPU)");
    options.AddOption("--use_user_supplied_buffers", "false", "Use user supplied input/output buffers");
    options.AddOption("--qnn_system_file", "libQnnSystem.so", "QNN system file path");
    options.AddOption("--cache_model_file", "./cache_model.bin", "Cache model file path");

    options.ParseArgs(argc, argv);

    return 0;
}

int main(int argc, const char* argv[]) {
    // 解析参数
    // 参数一：qnn backend文件路径
    // 参数二：qnn模型文件路径
    // 参数三：模型文件路径
    // 参数四：输入文件路径
    // 参数五：输出文件路径
    // 参数六：运行模式（CPU/GPU）
    // 参数七：是否使用用户提供的输入/输出缓冲区
    // 参数八：qnn系统文件路径
    // 参数九：缓存的模型文件路径

    // 解析参数
    CommandParser options;
    // Parse and check arguments
    int ret = ParseAndCheckArgs(argc, argv, options);
    if (ret != 0) {
        printf("ParseAndCheckArgs failed\n");
        return ret;
    }

    // std::string model_tmp = options.GetStringOption("--model");
    std::string qnn_backend_file = options.GetStringOption("--backend_file");
    std::string qnn_model_file = options.GetStringOption("--qnn_model_file");
    std::string qnn_modeldlc_file = options.GetStringOption("--qnn_modeldlc_file");
    std::string input_file = options.GetStringOption("--input_file");
    std::string output_file = options.GetStringOption("--output_file");
    std::string runtime = options.GetStringOption("--runtime");
    bool use_user_supplied_buffers = options.GetBoolOption("--use_user_supplied_buffers");
    std::string qnn_systefile = options.GetStringOption("--qnn_system_file");
    std::string cache_model_file = options.GetStringOption("--cache_model_file");

    // 打印参数
    printf("--------print parameters begin--------\n");
    printf("backend_file: %s\n", qnn_backend_file.c_str());
    printf("qnn_model_file: %s\n", qnn_model_file.c_str());
    printf("input_file: %s\n", input_file.c_str());
    printf("output_file: %s\n", output_file.c_str());
    printf("runtime: %s\n", runtime.c_str());
    printf("use_user_supplied_buffers: %d\n", use_user_supplied_buffers);
    printf("qnn_systefile: %s\n", qnn_systefile.c_str());
    printf("cache_model_file: %s\n", cache_model_file.c_str());
    printf("--------print parameters end--------\n");
  
    printf("---------------------Begin to test snpe runtime---------------------\n");
    // 1. Load QNN function pointers
    QnnFunctionPointers *qnnFunctionPointers;
    // 如果qnn_model_file为.dlc文件，则使用qnn_modeldlc_file
    if (qnn_model_file.find(".dlc") != std::string::npos) {
        ret = load_qnn_function_pointers(qnnFunctionPointers, qnn_backend_file.c_str(), qnn_modeldlc_file.c_str(), qnn_systefile.c_str(), qnn_modeldlc_file.c_str());
    } else {
        ret = load_qnn_function_pointers(qnnFunctionPointers, qnn_backend_file.c_str(), qnn_model_file.c_str(), qnn_systefile.c_str(), qnn_modeldlc_file.c_str());
    }
    if (ret != 0) {
        printf("Failed to load QNN function pointers.\n");
        return -1;
    }
    
    // 2. Create a QnnDevice handle to access device APIs
    Qnn_LogHandle_t logHandle;
    Qnn_GraphHandle_t graphHandle;
    Qnn_ContextHandle_t context;
    QnnContext_Config_t **contextConfigs;
    Qnn_DeviceHandle_t deviceHandle;
    Qnn_BackendHandle_t backendHandle;
    Qnn_ProfileHandle_t profileBackendHandle;

    ret = Init(qnnFunctionPointers, logHandle, backendHandle, deviceHandle, context, graphHandle, profileBackendHandle, cache_model_file.c_str());
    if (ret != 0) {
        printf("Failed to initialize QNN runtime.\n");
        return -1;
    }

    GraphInfo_t** graphsInfo;
    uint32_t graphsCount;
    bool used_cache = false;
    ret = load_model(qnnFunctionPointers, cache_model_file.c_str(), backendHandle, deviceHandle, profileBackendHandle, 
                    &graphHandle, context, contextConfigs, graphsInfo, graphsCount, qnn_model_file.c_str(), used_cache);
    if (ret != 0) {
        printf("Failed to load QNN model.\n");
        return -1;
    }

    Qnn_Tensor_t* input_tensors = nullptr;
    Qnn_Tensor_t* output_tensors = nullptr;
    uint32_t inputSize = 0;
    uint32_t outputSize = 0;
    Qnn_DataType_t input_dataType = QNN_DATATYPE_FLOAT_32;
    Qnn_DataType_t output_dataType = QNN_DATATYPE_FLOAT_32;
    uint32_t inputDataSize = 0;
    uint32_t outputDataSize = 0;
    ret = ready_tensors(input_tensors, output_tensors, inputSize, outputSize, input_dataType, output_dataType, 
                        inputDataSize, outputDataSize, graphHandle, context, graphsInfo);

    // 塞数据到inputs
    // 1. 读取输入文件
    ret = read_input_data(input_tensors, inputSize, inputDataSize, input_file.c_str());
    if (ret != 0) {
        printf("Failed to read input data.\n");
        return -1;
    }
    // printf("inputData: ")
    // for (size_t i = 0; i < inputSize; i++) {
    //     printf("%f ", inputData[i]);
    // }
    // printf("\n");
    printf("Copied input data to tensor\n");
    
    // 3. 塞到outputs
    // 4. 执行graph
    GraphInfo_t* graphInfo = graphsInfo[0];
    Qnn_ErrorHandle_t status = qnnFunctionPointers->qnnInterface.graphExecute(graphInfo,
                                                                              input_tensors,
                                                                              graphInfo->numInputTensors,
                                                                              output_tensors,
                                                                              graphInfo->numOutputTensors,
                                                                              profileBackendHandle,
                                                                              nullptr);
    if (QNN_GRAPH_NO_ERROR != status) {
        printf("Failed to execute graph");
        return -1;
    }
    printf("Successfully executed graph\n");
    // 5. 输出结果到文件
    ret = save_output_data(output_tensors, outputSize, outputDataSize, output_file.c_str());
    if (ret != 0) {
        printf("Failed to save output data.\n");
        return -1;
    }
    printf("Wrote output data to file: %s\n", output_file.c_str());

    // 测试,直接打印data
    // float * test_output_data = reinterpret_cast<float*>(QNN_TENSOR_GET_CLIENT_BUF(output_tensors[0]).data);
    // for (size_t i = 0; i < outputSize; i++) {
    //     printf("%f ", test_output_data[i]);
    // }
    // printf("\n");

    // 15. Free context
    if (QNN_CONTEXT_NO_ERROR !=
          qnnFunctionPointers->qnnInterface.contextFree(context, profileBackendHandle)) {
        printf("Could not free context");
        return -1;
    }
    // 16. terminate backend
    if (QNN_BACKEND_NO_ERROR != qnnFunctionPointers->qnnInterface.backendFree(backendHandle)) {
        printf("Could not free backend");
        return -1;
    }
    
    return 0;
}