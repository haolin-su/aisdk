#include "utils_opencv.h"

#include <vector>

#include "opencv2/opencv.hpp"
#include "opencv2/core.hpp"

namespace aisdk
{
    bool isMatValid(const cv::Mat& mat) {
        return !mat.empty();
    }

    cv::Mat squeeze(const cv::Mat& input) {
        std::vector<int> new_shape;
        for (int i = 0; i < input.dims; ++i) {
            if (input.size[i] > 1) {
                new_shape.push_back(input.size[i]);
            }
        }
        return input.reshape(1, new_shape);
    }

    cv::Mat apply_sigmoid(const cv::Mat& input) {
        cv::Mat output;
        cv::exp(-input, output);
        return 1.0 / (1.0 + output);
    }

    void BoxesVec2Rect(const std::vector<int32_t>& boxes, cv::Rect& rect) {
        rect.x = boxes[0];
        rect.y = boxes[1];
        rect.width = boxes[2];
        rect.height = boxes[3];
    }

    std::vector<cv::Rect> rescale_boxes(const std::vector<cv::Rect>& boxes, cv::Size original_size, cv::Size new_size) {
        std::vector<cv::Rect> scaled_boxes;
        float scale_x = static_cast<float>(new_size.width) / original_size.width;
        float scale_y = static_cast<float>(new_size.height) / original_size.height;

        for (const auto& box : boxes) {
            cv::Rect scaled_box(
                static_cast<int>(std::round(box.x * scale_x)),
                static_cast<int>(std::round(box.y * scale_y)),
                static_cast<int>(std::round(box.width * scale_x)),
                static_cast<int>(std::round(box.height * scale_y)));
            scaled_boxes.push_back(scaled_box);
        }
        return scaled_boxes;
    }
}