#include "utils_opencv.h"

#include <vector>
#include <iostream>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

namespace aisdk
{

    cv::Mat Squeeze(const cv::Mat &input)
    {
        std::vector<int> new_shape;
        for (int i = 0; i < input.dims; ++i)
        {
            if (input.size[i] > 1)
            {
                new_shape.push_back(input.size[i]);
            }
        }
        return input.reshape(1, new_shape);
    }

    cv::Mat ApplySigmoid(const cv::Mat &input)
    {
        cv::Mat output;
        cv::exp(-input, output);
        return 1.0 / (1.0 + output);
    }

    void BoxesVec2Rect(const std::vector<int32_t> &boxes, cv::Rect &rect)
    {
        rect.x = boxes[0];
        rect.y = boxes[1];
        rect.width = boxes[2];
        rect.height = boxes[3];
    }

    std::vector<std::vector<int32_t>> RescaleBoxes(const std::vector<std::vector<int32_t>> &boxes, int32_t input_height, int32_t input_width, int32_t output_height, int32_t output_width)
    {
        std::vector<std::vector<int32_t>> scaled_boxes;

        float scale_x = static_cast<float>(output_width) / input_width;
        float scale_y = static_cast<float>(output_height) / input_height;

        for (const auto &box : boxes)
        { // x, y, w, h
            std::vector<int32_t> scaled_box{
                static_cast<int32_t>(std::round(box[0] * scale_x)),
                static_cast<int32_t>(std::round(box[1] * scale_y)),
                static_cast<int32_t>(std::round(box[2] * scale_x)),
                static_cast<int32_t>(std::round(box[3] * scale_y))};
            scaled_boxes.push_back(scaled_box);
        }
        return scaled_boxes;
    }

    void PrintMatShape(const cv::Mat &mat)
    {

        std::cout << "Shape: [";
        for (int i = 0; i < mat.dims; ++i)
        {
            std::cout << mat.size[i];
            if (i < mat.dims - 1)
            {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }
}