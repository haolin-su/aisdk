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

    // std::vector<int> input_shape_1 = {1, 116, 8400};
    // std::vector<int> input_shape_2 = {1, 32, 160, 160};

    // TensorShape tensor_shape_0{3, {1, 116, 8400}};
    // TensorShape tensor_shape_1{4, {1, 32, 160, 160}};
    // float* data;
    // tensor->GetData(data);
    // mat = cv::Mat(640, 640, CV_32F, data);

    return 0;
}

int mat2tensor(const cv::Mat& mat, TensorPtr& tensor, bool is_copy) {

    if (mat.empty()) {
        return -1;
    }

    if (tensor == nullptr) {
        tensor = std::make_shared<Tensor>();
    }



    //     std::cout << "Shape: [";
    //     for (int i = 0; i < mat.dims; ++i)
    //     {
    //         std::cout << mat.size[i];
    //         if (i < mat.dims - 1)
    //         {
    //             std::cout << ", ";
    //         }
    //     }
    //     std::cout << "]" << std::endl;

    // tensor->SetData(mat.data);
    
    // TensorShape tensor_shape{4, {batchs, height, width, channels}};
    // tensor->SetShape(tensor_shape);

    // if (is_copy) {
    //     tensor = Tensor(mat.rows, mat.cols, mat.type(), mat.data);
    // } else {
    //     tensor = Tensor(mat.rows, mat.cols, mat.type(), mat.data);
    // }

    return 0;
} 
} 