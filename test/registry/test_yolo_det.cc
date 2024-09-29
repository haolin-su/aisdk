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
#include "core/tensor/tensor.h"

int main(int argc, char* argv[]) {
    std::cout << "test yolo det" << std::endl;

    aisdk::YoloPostProcNode yolo_postproc_node;
    yolo_postproc_node.Init("config");
    std::shared_ptr<aisdk::Tensor> input_tensor = std::make_shared<aisdk::Tensor>();
    std::shared_ptr<aisdk::Tensor> output_tensor = std::make_shared<aisdk::Tensor>();
    yolo_postproc_node.Process(input_tensor, output_tensor);

    return 0;
}