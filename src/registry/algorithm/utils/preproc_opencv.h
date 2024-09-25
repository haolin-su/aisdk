#ifndef FIBO_AI_PREPROC_OPENCV_H_
#define FIBO_AI_PREPROC_OPENCV_H_

// #include "preproc.h"
#include "opencv2/opencv.hpp"

namespace aisdk {

int ResnetPreprocOpencv(const cv::Mat& input_mat, cv::Mat& output_mat, int model_h, int model_w);
int YoloPreprocOpencv(const cv::Mat& input_mat, cv::Mat& output_mat, int model_h, int model_w);


} // namespace aisdk

#endif // FIBO_AI_PREPROC_OPENCV_H_