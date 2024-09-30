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

#include "registry/algorithm/cv_detection_yolo_postproc.h"
#include "registry/algorithm/cv_segmentation_yolo_postproc.h"
#include "core/tensor/tensor.h"
#include "core/utils.h"

void read_tensor_raw(std::string& file_path, std::vector<int>&input_shape, std::shared_ptr<aisdk::Tensor>& tensor)
{
    // 设定矩阵的尺寸和类型
    int batchs = input_shape[0]; // 批次数
    int channels = input_shape[1]; // 通道数
    int height = input_shape[2]; // 高度
    int width = input_shape[3];  // 宽度

    // 创建一个 cv::Mat 以存储读取的数据
    cv::Mat input_tensor(height, width, CV_32FC3); // 3 通道浮点型

    // 打开 .raw 文件以二进制方式读取
    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error opening " << file_path << std::endl;
        return; // 错误处理
    }

    // 读取数据
    file.read(reinterpret_cast<char*>(input_tensor.data), input_tensor.total() * input_tensor.elemSize());
    
    // 检查读取是否成功
    if (!file) {
        std::cerr << "Error reading from " << file_path << std::endl;
        return; // 错误处理
    }

    file.close(); // 关闭文件

    tensor->SetData(input_tensor.data);
    aisdk::TensorShape tensor_shape{4, {batchs, height, width, channels}};
    tensor->SetShape(tensor_shape);
}

int main(int argc, char* argv[]) {
    std::cout << "test yolo det" << std::endl;

    aisdk::YoloPostProcNode yolo_postproc_node;
    aisdk::YoloSegPostProcNode yolo_seg_postproc_node;
    yolo_postproc_node.Init("config");
    std::shared_ptr<aisdk::Tensor> input_tensor = std::make_shared<aisdk::Tensor>();
    std::shared_ptr<aisdk::Tensor> output_tensor = std::make_shared<aisdk::Tensor>();

    std::string file_path = "input_tensor.raw";
    std::vector<int> input_shape = {1, 3, 640, 640};
    read_tensor_raw(file_path, input_shape, input_tensor);

    yolo_postproc_node.Process(input_tensor, output_tensor);
    std::vector<std::shared_ptr<aisdk::Tensor>> input_tensors;
    std::vector<std::shared_ptr<aisdk::Tensor>> output_tensors;
    input_tensors.push_back(input_tensor);
    input_tensors.push_back(input_tensor);
    output_tensors.push_back(output_tensor);
    output_tensors.push_back(output_tensor);
    yolo_seg_postproc_node.Process(input_tensors, output_tensors);

    return 0;
}