#include "postproc_opencv.h"

#include <vector>
#include "opencv2/dnn.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include "core/tensor/tensor.h"
#include "common/tensor_util.h"

namespace aisdk
{
    bool isMatValid(const cv::Mat& mat) {
        return !mat.empty() && mat.total() > 0;
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

    void BoxesVec2Rect(const std::vector<int32_t>& boxes, cv::Rect& rect){
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

    // May implement other NMS algorithms here
    std::vector<int32_t> NMSBoxes(const std::vector<std::vector<int32_t>>& boxes, const std::vector<float>& scores, const float confidence_threshold, const float iou_threshold)
    {
        std::cout << "NMSBoxes" << std::endl;
        std::vector<int32_t> indices;

        std::vector<cv::Rect> rects(boxes.size());
        for (int32_t i = 0; i < boxes.size(); ++i) {
            BoxesVec2Rect(boxes[i], rects[i]);
        }
        cv::dnn::NMSBoxes(rects, scores, confidence_threshold, iou_threshold, indices);

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
        // int32_t left = static_cast<int32_t>((x - w / 2) * x_factor);
        // int32_t top = static_cast<int32_t>((y - h / 2) * y_factor);
        // int32_t width = static_cast<int32_t>(w * x_factor);
        // int32_t height = static_cast<int32_t>(h * y_factor);
        int32_t left = static_cast<int32_t>(x - w / 2);
        int32_t top = static_cast<int32_t>(y - h / 2);
        int32_t width = static_cast<int32_t>(w);
        int32_t height = static_cast<int32_t>(h);

        return std::vector<int32_t>{left, top, width, height};
    }

    int32_t ProcessDetectionPredictions(const cv::Mat& results, float confidence_threshold, Predictions& predictions)
    {
        std::cout << "ProcessDetectionPredictions" << std::endl;
        if(!isMatValid(results)){
            std::cout << "Invalid results" << std::endl;
            return -1;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n
        cv::Mat output = squeeze(results);
        cv::transpose(output, output);

        // Iterate over each row of the output
        for (int32_t i = 0; i < output.rows; ++i)
        {
            cv::Mat class_scores = output.row(i).colRange(4, output.cols);
            double max_score;
            int32_t max_index;

            // Find the maximum score and its index
            cv::minMaxIdx(class_scores, nullptr, &max_score, nullptr, &max_index);

            // If the maximum score is above the confidence threshold
            if (max_score >= confidence_threshold) {
                // Add the class ID, score, and box coordinates to the respective lists
                predictions.class_ids.push_back(max_index);
                predictions.scores.push_back(static_cast<float>(max_score));
                predictions.boxes.push_back(ExtractBox(output, i));
            }
        }
        return predictions.class_ids.empty() ? -1 : 0;
    }

    void ConstructResultTensor(const Predictions& predictions, std::vector<int>& indices, cv::Mat& output)
    {
        std::cout << "ConstructResultTensor" << std::endl;

        if (predictions.class_ids.empty() || indices.empty()){
            return;
        }
        // accroding to the result.h, the max number of boxes is 32
        int max_boxes = std::min(static_cast<int>(indices.size()), 32);
        // each row: class_id, score, bbox->(left, top, width, height）

        output.create(std::vector<int32_t>{max_boxes, 6}, CV_32FC1);

        for (int32_t i = 0; i < max_boxes; ++i) {
            int32_t index = indices[i];
            output.at<float>(i, 0) = static_cast<float>(predictions.class_ids[index]);
            output.at<float>(i, 1) = static_cast<float>(predictions.scores[index]);
            
            const auto& box = predictions.boxes[index];
            output.at<float>(i, 2) = static_cast<float>(box[0]);
            output.at<float>(i, 3) = static_cast<float>(box[1]);
            output.at<float>(i, 4) = static_cast<float>(box[2]);
            output.at<float>(i, 5) = static_cast<float>(box[3]);
        }     
    }

    int32_t YoloDetectionPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold)
    {
        std::cout << "YoloDetectionPostprocOpencv" << std::endl;

        if(!isMatValid(input) || !isMatValid(output)){
            std::cout << "Invalid input or output" << std::endl;
            return -1;
        }

        Predictions predictions;
        if(ProcessDetectionPredictions(input, confidence_threshold, predictions) < 0){
            return -1;
        }

        if (predictions.class_ids.empty()) {
            output.release();
            return 0;
        }

        // TODO: 这里的IOU_THRESHOLD需要根据情况调整
        // Apply non-maximum suppression to filter out overlapping bounding boxes
        std::vector<int> indices = NMSBoxes(predictions.boxes, predictions.scores, confidence_threshold, IOU_THRESHOLD);

        ConstructResultTensor(predictions, indices, output);

        return 0;
    }

    int32_t ProcessSegmentationPredictions(const cv::Mat& results, float confidence_threshold, int32_t num_masks, Predictions& predictions, MaskPredictions& mask_predictions)
    {
        std::cout << "ProcessSegmentationPredictions" << std::endl;
        if(!isMatValid(results)){
            std::cout << "Invalid results" << std::endl;
            return -1;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n, mask_1, mask_2,..., mask_n
        cv::Mat output = squeeze(results);
        cv::transpose(output, output); 
        // Total size = bbox (bbox_x, bbox_y, bbox_w, bbox_h) + num_classes + num_masks
        int32_t num_classes = results.size[1] - num_masks - 4;

        // Iterate over each row of the output
        for (int32_t i = 0; i < output.rows; ++i)
        {
            cv::Mat class_scores = output.row(i).colRange(4, 4 + num_classes);
            double max_score;
            int32_t max_index;

            // Find the maximum score and its index
            cv::minMaxIdx(class_scores, nullptr, &max_score, nullptr, &max_index);

            // If the maximum score is above the confidence threshold
            if (max_score >= confidence_threshold) {
                // Add the class ID, score, and box coordinates to the respective lists
                predictions.class_ids.push_back(max_index);
                predictions.scores.push_back(static_cast<float>(max_score));
                predictions.boxes.push_back(ExtractBox(output, i));
                mask_predictions.mask_predictions.push_back(output.row(i).colRange(4 + num_classes, output.cols));
            }
        }
        return predictions.class_ids.empty() ? -1 : 0;
    }

    // TODO: 这段代码要重新看一下实现
    int32_t ProcessSegmentationMaskPredictions(const cv::Mat& results, MaskPredictions& mask_predictions, std::vector<std::vector<int32_t>> boxes, cv::Mat& mask_map)
    {
        std::cout << "ProcessSegmentationMaskPredictions" << std::endl;
        if(!isMatValid(results) || !isMatValid(mask_predictions.mask_predictions[0])){
            std::cout << "Invalid input" << std::endl;
            return -1;
        }

        cv::Mat output = squeeze(mask_predictions.mask_predictions[0]);

        int32_t num_mask = mask_predictions.mask_predictions[0].at<float>(0);
        int32_t mask_height = mask_predictions.mask_predictions[0].at<float>(1);
        int32_t mask_width = mask_predictions.mask_predictions[0].at<float>(2);

        // 计算掩码
        cv::Mat masks;
        cv::Mat reshaped_mask_output = output.reshape(1, num_mask); // CHW 转换为 C(高度*宽度)
        cv::Mat sigmoid_mask_predictions = apply_sigmoid(mask_predictions.mask_predictions[0] * reshaped_mask_output);
        masks = sigmoid_mask_predictions.reshape(1, -1); // 重新调整形状

        // 调整边界框
        std::vector<cv::Rect> rects(boxes.size());
        for (int i = 0; i < boxes.size(); ++i) {
            BoxesVec2Rect(boxes[i], rects[i]);
        }

        // TODO: 这里的img_height和img_width需要从算法的信息获取
        int32_t img_height = 416;
        int32_t img_width = 416;
        std::vector<cv::Rect> scale_boxes = rescale_boxes(rects, cv::Size(img_height, img_width), cv::Size(mask_height, mask_width));

        // 处理每个框/掩码对
        mask_map = cv::Mat::zeros(scale_boxes.size(), img_height * img_width, CV_32F);
        cv::Size blur_size(int(img_width / mask_width), int(img_height / mask_height));

        for (size_t i = 0; i < scale_boxes.size(); ++i) {
            int scale_x1 = std::floor(scale_boxes[i].x);
            int scale_y1 = std::floor(scale_boxes[i].y);
            int scale_x2 = std::ceil(scale_boxes[i].x + scale_boxes[i].width);
            int scale_y2 = std::ceil(scale_boxes[i].y + scale_boxes[i].height);

            int x1 = std::floor(boxes[i][0]);
            int y1 = std::floor(boxes[i][1]);
            int x2 = std::ceil(boxes[i][0] + boxes[i][2]);
            int y2 = std::ceil(boxes[i][1] + boxes[i][3]);

            cv::Mat scale_crop_mask = masks.row(i)(cv::Range(scale_y1, scale_y2), cv::Range(scale_x1, scale_x2));
            cv::Mat crop_mask;
            cv::resize(scale_crop_mask, crop_mask, cv::Size(x2 - x1, y2 - y1), 0, 0, cv::INTER_CUBIC);
            cv::blur(crop_mask, crop_mask, blur_size);

            cv::threshold(crop_mask, crop_mask, 0.5, 1, cv::THRESH_BINARY);
            mask_map(std::vector<cv::Range>{cv::Range(i, i + 1), cv::Range(y1, y2), cv::Range(x1, x2)}) = crop_mask;
        }
        return 0;
    }

    int32_t YoloSegmentationPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold, int32_t num_masks)
    {
        std::cout << "YoloSegmentationPostprocOpencv" << std::endl;

        if(!isMatValid(input) || !isMatValid(output)){
            std::cout << "Invalid input or output" << std::endl;
            return -1;
        }

        cv::Mat prediction_input = input(std::vector<cv::Range>{cv::Range(0, 1), cv::Range::all(), cv::Range::all(), cv::Range::all()});
        cv::Mat mask_input = input(std::vector<cv::Range>{cv::Range(1, 2), cv::Range::all(), cv::Range::all(), cv::Range::all()});

        Predictions predictions;
        MaskPredictions mask_predictions;
        if(ProcessSegmentationPredictions(prediction_input, confidence_threshold, NUM_MASKS, predictions, mask_predictions) < 0){
            return -1;
        }

        if (predictions.class_ids.empty() || mask_predictions.mask_predictions.empty()) {
            output.release();
            return 0;
        }

        cv::Mat mask_map;
        if(ProcessSegmentationMaskPredictions(mask_input, mask_predictions, predictions.boxes, mask_map) < 0){
            return -1;
        }

        // TODO: 这里的IOU_THRESHOLD需要根据情况调整
        // Apply non-maximum suppression to filter out overlapping bounding boxes
        std::vector<int> indices = NMSBoxes(predictions.boxes, predictions.scores, confidence_threshold, IOU_THRESHOLD);

        cv::Mat prediction_output;
        ConstructResultTensor(predictions, indices, prediction_output);

        output(std::vector<cv::Range>{cv::Range(0, 1), cv::Range::all(), cv::Range::all(), cv::Range::all()}) = prediction_output;
        output(std::vector<cv::Range>{cv::Range(1, 2), cv::Range::all(), cv::Range::all(), cv::Range::all()}) = mask_map;

        return 0;
    }

}