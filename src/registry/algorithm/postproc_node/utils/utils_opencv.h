#ifndef FIBO_AI_UTILS_OPENCV_H_
#define FIBO_AI_UTILS_OPENCV_H_

#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

namespace aisdk {
    cv::Mat Squeeze(const cv::Mat& input);
    cv::Mat ApplySigmoid(const cv::Mat& input);
    void BoxesVec2Rect(const std::vector<int32_t>& boxes, cv::Rect& rect);
    std::vector<std::vector<int32_t>> RescaleBoxes(const std::vector<std::vector<int32_t>> &boxes, const int32_t image_height, 
                                                    const int32_t image_width, const int32_t input_height, const int32_t input_width);
    // for debug
    void PrintMatShape(const cv::Mat &mat);
    void SaveMatAsRaw(const cv::Mat &mat, const std::string &filename);
} // namespace aisdk

#endif // FIBO_AI_UTILS_OPENCVH_