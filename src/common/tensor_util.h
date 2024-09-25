#ifndef FIBO_TENSOR_UTIL_H
#define FIBO_TENSOR_UTIL_H

#include "core/tensor/tensor.h"
#include "opencv2/opencv.hpp"

namespace aisdk {

int tensor2mat(const TensorPtr& tensor, cv::Mat& mat, bool is_copy = false);
int mat2tensor(const cv::Mat& mat, TensorPtr& tensor, bool is_copy = false);

}

#endif // FIBO_TENSOR_UTIL_H