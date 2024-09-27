#ifndef FIBO_AI_POSTPROC_OPENCV_H_
#define FIBO_AI_POSTPROC_OPENCV_H_

// #include "postproc.h"
#include <vector>

#include "opencv2/opencv.hpp"

namespace aisdk {

#define CONFIDENCE_THRESHOLD (0.5)
#define IOU_THRESHOLD   (0.45)
#define NUM_MASKS       (32)

struct Predictions {
    std::vector<int32_t> class_ids;
    std::vector<float> scores;
    std::vector<std::vector<int32_t>> boxes;
};

struct MaskPredictions {
    std::vector<cv::Mat> mask_predictions;
};

int32_t YoloDetectionPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold);
int32_t YoloSegmentationPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold, int32_t num_masks);

} // namespace aisdk

#endif // FIBO_AI_POSTPROC_OPENCV_H_