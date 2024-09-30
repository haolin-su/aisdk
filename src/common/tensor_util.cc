#include "tensor_util.h"

#include <opencv2/opencv.hpp>

namespace aisdk{

int tensor2mat(const TensorPtr& tensor, cv::Mat& mat, bool is_copy) {

    if (tensor == nullptr) {
        return -1;
    }

    // if (is_copy) {
    //     mat = cv::Mat(tensor->height(), tensor->width(), tensor->type(), tensor->data());
    // } else {
    //     mat = cv::Mat(tensor->height(), tensor->width(), tensor->type(), tensor->data());
    // }

    return 0;
}

int mat2tensor(const cv::Mat& mat, TensorPtr& tensor, bool is_copy) {

    if (mat.empty()) {
        return -1;
    }

    if (tensor == nullptr) {
        tensor = std::make_shared<Tensor>();
    }


    // if (is_copy) {
    //     tensor = Tensor(mat.rows, mat.cols, mat.type(), mat.data);
    // } else {
    //     tensor = Tensor(mat.rows, mat.cols, mat.type(), mat.data);
    // }

    return 0;
} 
} 