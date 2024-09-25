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

#include "DlSystem/UserBufferMap.hpp"
#include "DlSystem/IUserBuffer.hpp"
#include "DlSystem/DlEnums.hpp"
#include "DlSystem/RuntimeList.hpp"
#include "DlContainer/IDlContainer.hpp"

#include "SNPE/SNPE.hpp"
#include "SNPE/SNPEBuilder.hpp"
#include "SNPE/SNPEFactory.hpp"
#include "DiagLog/IDiagLog.hpp"

#include "preproc_opencv.h"
#include "yolo_preproc.h"

size_t calcSizeFromDims(const zdl::DlSystem::Dimension *dims, size_t rank, size_t elementSize )
{
    size_t resizable_dim;
   if (rank == 0) return 0;
   size_t size = elementSize;
   while (rank--) {
      (*dims == 0) ? size *= resizable_dim : size *= *dims;
      dims++;
   }
   return size;
}

void createUserBuffer(zdl::DlSystem::UserBufferMap& userBufferMap,
                      std::unordered_map<std::string, std::vector<uint8_t>>& applicationBuffers,
                      std::vector<std::unique_ptr<zdl::DlSystem::IUserBuffer>>& snpeUserBackedBuffers,
                      std::unique_ptr<zdl::SNPE::SNPE>& snpe,
                      const char * name,
                      const bool isTfNBuffer,
                      bool staticQuantization,
                      int bitWidth)
{
   // get attributes of buffer by name
   auto bufferAttributesOpt = snpe->getInputOutputBufferAttributes(name);
   if (!bufferAttributesOpt) throw std::runtime_error(std::string("Error obtaining attributes for input tensor ") + name);
   // calculate the size of buffer required by the input tensor
   const zdl::DlSystem::TensorShape& bufferShape = (*bufferAttributesOpt)->getDims();

   // calculate stride based on buffer strides
   // Note: Strides = Number of bytes to advance to the next element in each dimension.
   // For example, if a float tensor of dimension 2x4x3 is tightly packed in a buffer of 96 bytes, then the strides would be (48,12,4)
   std::vector<size_t> strides(bufferShape.rank());
   strides[strides.size() - 1] = sizeof(float);
   size_t stride = strides[strides.size() - 1];
   for (size_t i = bufferShape.rank() - 1; i > 0; i--)
   {
      stride *= bufferShape[i];
      strides[i-1] = stride;
   }

   const size_t bufferElementSize = (*bufferAttributesOpt)->getElementSize();
   size_t bufSize = calcSizeFromDims(bufferShape.getDimensions(), bufferShape.rank(), bufferElementSize);

   // set the buffer encoding type
   zdl::DlSystem::UserBufferEncodingFloat userBufferEncodingFloat;
   zdl::DlSystem::UserBufferSourceGLBuffer userBufferSourceGLBuffer;

   // create user-backed storage to load input data onto it
   applicationBuffers.emplace(name, 1);

   // create SNPE user buffer from the user-backed buffer
   zdl::DlSystem::IUserBufferFactory& ubFactory = zdl::SNPE::SNPEFactory::getUserBufferFactory();
   snpeUserBackedBuffers.push_back(ubFactory.createUserBuffer(&applicationBuffers.at(name),
                                                              bufSize,
                                                              strides,
                                                              &userBufferEncodingFloat,
                                                              &userBufferSourceGLBuffer));

   // add the user-backed buffer to the inputMap, which is later on fed to the network for execution
   userBufferMap.add(name, snpeUserBackedBuffers.back().get());
}

int main(int argc, char* argv[]) {
    // 解析参数
    // 参数一：模型文件路径
    // 参数二：输入文件路径
    // 参数三：输出文件路径
    // 参数四：运行模式（CPU/GPU）
    // 参数五：是否使用用户提供的输入/输出缓冲区
    if (argc < 5) {
        printf("Usage: %s <model_file> <input_file> <output_dir> <runtime> <use_user_supplied_buffers>\n", argv[0]);
        return 1;
    }

    const char* model_file = argv[1];
    const char* input_file = argv[2];
    const char* output_dir = argv[3];
    const char* runtime = argv[4];
    bool use_user_supplied_buffers = (bool)atoi(argv[5]);

    printf("Begin to test snpe runtime\n");

    // 1. 加载模型文件
    // 2. 设置运行模式
    // 3. 设置是否使用用户提供的输入/输出缓冲区
    // 4. 设置输入/输出张量形状
    // 5. 加载输入文件
    // 6. 执行模型推理
    // 7. 保存输出文件

    // 1. 加载模型文件
    std::unique_ptr<zdl::DlContainer::IDlContainer> container = zdl::DlContainer::IDlContainer::open(zdl::DlSystem::String(model_file));
    if (container == nullptr)
    {
        std::cerr << "Error while opening the container file." << std::endl;
        return EXIT_FAILURE;
    }

    // 2. 设置运行模式
    DlSystem::RuntimeList runtimeList;
    runtimeList.add(DlSystem::Runtime_t::CPU);
    runtimeList.add(DlSystem::Runtime_t::GPU);
    runtimeList.add(DlSystem::Runtime_t::DSP);

    enum {UNKNOWN, USERBUFFER_FLOAT, USERBUFFER_TF8, ITENSOR, USERBUFFER_TF16};
    int bufferType = USERBUFFER_FLOAT;
    bool useUserSuppliedBuffers = (bufferType == USERBUFFER_FLOAT || bufferType == USERBUFFER_TF8 || bufferType == USERBUFFER_TF16);
    bool useCaching = false;
    bool cpuFixedPointMode = false;

    zdl::DlSystem::PlatformConfig platformConfig;
    zdl::SNPE::SNPEBuilder snpeBuilder(container.get());
    std::unique_ptr<zdl::SNPE::SNPE> snpe = snpeBuilder.setOutputLayers({})
                                    .setRuntimeProcessorOrder(runtimeList)
                                    .setUseUserSuppliedBuffers(useUserSuppliedBuffers)
                                    .setPlatformConfig(platformConfig)
                                    .setInitCacheMode(useCaching)
                                    .setCpuFixedPointMode(cpuFixedPointMode)
                                    .build();
    
    if (snpe == nullptr)
    {
        std::cerr << "Error while building SNPE object." << std::endl;
        return EXIT_FAILURE;
    }

    // 3. 设置是否使用用户提供的输入/输出缓冲区
    zdl::DlSystem::TensorShape tensorShape;
    tensorShape = snpe->getInputDimensions();
    size_t batchSize = tensorShape.getDimensions()[0];
    size_t input_height = tensorShape.getDimensions()[2];
    size_t input_width = tensorShape.getDimensions()[3];

    // Open the input file listing and group input files into batches
    zdl::DlSystem::UserBufferMap inputMap, outputMap;   // 最终作为模型推理的输入/输出张量

    // 根据模型的输入输出的维度信息，计算需要分配的空间大小，分配空间
    std::vector <std::unique_ptr<zdl::DlSystem::IUserBuffer>> snpeUserBackedInputBuffers, snpeUserBackedOutputBuffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationOutputBuffers;
    std::unordered_map <std::string, std::vector<uint8_t>> applicationInputBuffers;

    // get input tensor names of the network that need to be populated
    const auto& inputNamesOpt = snpe->getInputTensorNames();
    if (!inputNamesOpt) throw std::runtime_error("Error obtaining input tensor names");
    const zdl::DlSystem::StringList& inputNames = *inputNamesOpt;

    // create SNPE user buffers for each application storage buffer
    bool staticQuantization = false;
    int bitWidth = 8;
    for (const char *name : inputNames) {
        createUserBuffer(inputMap, applicationInputBuffers, snpeUserBackedInputBuffers, snpe, name, true, staticQuantization, bitWidth);
    }
    
    // get input tensor names of the network that need to be populated
    const auto& outputNamesOpt = snpe->getOutputTensorNames();
    if (!outputNamesOpt) throw std::runtime_error("Error obtaining output tensor names");
    const zdl::DlSystem::StringList& outputNames = *outputNamesOpt;

    // create SNPE user buffers for each application storage buffer
    for (const char *name : outputNames) {
        createUserBuffer(outputMap, applicationOutputBuffers, snpeUserBackedOutputBuffers, snpe, name, true, false, bitWidth);
    }

    // 4. 填充具体的数据
    // 修改：这里假设传进来的文件都是图像文件，先进行前处理成raw数据，再接入到buffer里面去
    // std::ifstream inputList(input_file);
    // std::vector<std::vector<cv::Mat>> images;
    // int32_t ret = preprocessInput(inputList, batchSize, &images, input_width, input_height, true);
    //////////////////////////////////////////////////////////////////////////////////////////////////
    std::string inputText = "./test.txt";

    aisdk::ImageHandler handler;

    // 注册YOLOv8物体检测处理器
    handler.AddProcessor(std::make_shared<aisdk::YoloProcess>("yolov8_model.onnx"));

    // 处理文本中的图像路径
    handler.ProcessImages(inputText);
////////////////////////////////////////////////////////////////////////////////////////////////////////


    std::vector<std::string> inputs = {std::string(input_file)};
    // if(!loadInputUserBufferTfN(applicationInputBuffers, snpe, inputs[0], inputMap, staticQuantization, bitWidth))
    // {
    //     return EXIT_FAILURE;
    // }


    // 5. 执行模型推理
    bool execStatus = snpe->execute(inputMap, outputMap);
    if (execStatus == false)
    {
        std::cerr << "Error while executing the network." << std::endl;
        return EXIT_FAILURE;
    }

    // 6. 保存输出文件
    if (execStatus == true)
    {
        // if(!saveOutput(outputMap, applicationOutputBuffers, OutputDir, i * batchSize, batchSize, true, bitWidth))
        {
            return EXIT_FAILURE;
        }
    }

    return 0;
}