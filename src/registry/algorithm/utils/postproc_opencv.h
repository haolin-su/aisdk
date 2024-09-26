#ifndef FIBO_AI_POSTPROC_OPENCV_H_
#define FIBO_AI_POSTPROC_OPENCV_H_

// #include "postproc.h"
#include <vector>

#include "opencv2/opencv.hpp"

namespace aisdk {

#define IOU_THRESHOLD (0.45)

struct Predictions {
    std::vector<int32_t> class_ids;
    std::vector<float> scores;
    std::vector<std::vector<int32_t>> boxes;
};

int YoloPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold);


} // namespace aisdk

#endif // FIBO_AI_POSTPROC_OPENCV_H_