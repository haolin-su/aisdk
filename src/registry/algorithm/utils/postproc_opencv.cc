#include "postproc_opencv.h"

#include <vector>
#include "opencv2/dnn.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include "core/tensor/tensor.h"
#include "common/tensor_util.h"

namespace aisdk
{
    // May implement other NMS algorithms here
    std::vector<int32_t> NMSBoxes(const Predictions& predictions, float confidence_threshold, float iou_threshold)
    {
        std::cout << "NMSBoxes" << std::endl;
        std::vector<int32_t> indices;

        cv::dnn::NMSBoxes(predictions.boxes, predictions.scores, confidence_threshold, iou_threshold, indices);

        return indices;
    }

    std::vector<int32_t> ExtractBox(const cv::Mat& output, int32_t i)
    {
        std::cout << "ExtractBox" << std::endl;
        float x = output.at<float>(i, 0);
        float y = output.at<float>(i, 1);
        float w = output.at<float>(i, 2);
        float h = output.at<float>(i, 3);

        // Calculate the scaled coordinates of the bounding box
        // TODO: 这里需要根据模型的输入尺寸进行调整
        // int left = static_cast<int>((x - w / 2) * x_factor);
        // int top = static_cast<int>((y - h / 2) * y_factor);
        // int width = static_cast<int>(w * x_factor);
        // int height = static_cast<int>(h * y_factor);
        int32_t left = static_cast<int>(x - w / 2);
        int32_t top = static_cast<int>(y - h / 2);
        int32_t width = static_cast<int>(w);
        int32_t height = static_cast<int>(h);

        return std::vector<int32_t>{left, top, width, height};
    }

    void ProcessPredictions(const cv::Mat& results, float confidence_threshold, Predictions& predictions)
    {
        std::cout << "ProcessPredictions" << std::endl;

        // Transpose and squeeze the results tensor to get a 2D matrix of shape [num_boxes, num_classes + 5]
        cv::Mat reshaped_output = results.reshape(1, results.size[1]);
        cv::transpose(reshaped_output, reshaped_output);
        reshaped_output = reshaped_output.reshape(1, reshaped_output.size[0]);

        // Iterate over each row of the output
        for (int32_t i = 0; i < reshaped_output.rows; ++i)
        {
            cv::Mat class_scores = reshaped_output.row(i).colRange(4, reshaped_output.cols);
            double max_score;
            int32_t max_index;

            // Find the maximum score and its index
            cv::minMaxIdx(class_scores, nullptr, &max_score, nullptr, &max_index);

            // If the maximum score is above the confidence threshold
            if (max_score >= confidence_threshold) {
                // Get the class ID with the highest score
                int32_t class_id = max_index;

                // Extract the bounding box coordinates
                std::vector<int32_t> box = ExtractBox(reshaped_output, i);

                // Add the class ID, score, and box coordinates to the respective lists
                predictions.class_ids.push_back(class_id);
                predictions.scores.push_back(static_cast<float>(max_score));
                predictions.boxes.push_back(box);
            }
        }
    }

    void ConstructResultTensor(const Predictions& predictions, std::vector<int>& indices, cv::Mat& output)
    {
        std::cout << "ConstructResultTensor" << std::endl;

        output.release();  

        if (predictions.class_ids.empty() || indices.empty()){
            return;           
        } else if (indices.size() > 32) {
            indices.resize(32);
        }

        std::vector<int>shape {int32_t(indices.size()), 6};
        // each row: class_id, score, bbox->(left, top, width, height）
        output.create(shape, CV_32FC1);
        int32_t i = 0;

        for (int index : indices) {
            output.at<float>(i, 0) = static_cast<float>(predictions.class_ids[index]);
            output.at<float>(i, 1) = static_cast<float>(predictions.scores[index]);
            
            const auto& box = predictions.boxes[index];
            output.at<float>(i, 2) = static_cast<float>(box[0]);
            output.at<float>(i, 3) = static_cast<float>(box[1]);
            output.at<float>(i, 4) = static_cast<float>(box[2]);
            output.at<float>(i, 5) = static_cast<float>(box[3]);

            ++i;
        }     
    }

    int32_t YoloPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold)
    {
        std::cout << "YoloPostprocOpencv" << std::endl;
        Predictions predictions;
        ProcessPredictions(input, confidence_threshold, predictions);
        if (predictions.class_ids.empty()) {
            return 0;
        }

        // TODO: 这里的IOU_THRESHOLD需要根据情况调整
        // Apply non-maximum suppression to filter out overlapping bounding boxes
        std::vector<int> indices = NMSBoxes(predictions, confidence_threshold, IOU_THRESHOLD);

        ConstructResultTensor(predictions, indices, output);

        return 0;
    }

}