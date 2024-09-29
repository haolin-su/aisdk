#ifndef FIBO_AI_UTILS_OPENCV_H_
#define FIBO_AI_UTILS_OPENCV_H_

#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

namespace aisdk {
    bool isMatValid(const cv::Mat& mat);
    cv::Mat squeeze(const cv::Mat& input);
    cv::Mat apply_sigmoid(const cv::Mat& input);
    void BoxesVec2Rect(const std::vector<int32_t>& boxes, cv::Rect& rect);
    std::vector<cv::Rect> rescale_boxes(const std::vector<cv::Rect>& boxes, cv::Size original_size, cv::Size new_size);

} // namespace aisdk

#endif // FIBO_AI_UTILS_OPENCVH_