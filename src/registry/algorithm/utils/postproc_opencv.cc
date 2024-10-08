#include "postproc_opencv.h"

#include <vector>
#include <iostream>
#include "opencv2/dnn.hpp"
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#include "utils_opencv.h"
#include "core/tensor/tensor.h"
#include "error.h"


namespace aisdk
{
    int32_t NMSBoxes(std::vector<std::vector<int32_t>>& boxes, const std::vector<float>& scores, 
                    const float confidence_threshold, const float iou_threshold, std::vector<int32_t> &indices)
    {
        if(boxes.empty() || scores.empty() || boxes.size()!= scores.size() ||
            confidence_threshold < 0.0 || confidence_threshold > 1.0 ||
            iou_threshold < 0.0 || iou_threshold > 1.0){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // Use OpenCV's built-in non-maximum suppression algorithm, may implement other NMS algorithms here
        std::vector<cv::Rect> rects(boxes.size());
        for (int32_t i = 0; i < boxes.size(); ++i) {
            BoxesVec2Rect(boxes[i], rects[i]);
        }
        cv::dnn::NMSBoxes(rects, scores, confidence_threshold, iou_threshold, indices);

        // accroding to the result.h, the max number of boxes is 32
        int32_t max_boxes = std::min(static_cast<int>(indices.size()), 32);
        indices.resize(max_boxes);

        boxes.clear();
        boxes.reserve(max_boxes);

        for (int32_t i = 0; i < max_boxes; ++i) {
            int32_t index = indices[i];
            boxes.push_back(std::vector<int32_t>{rects[index].x, rects[index].y, rects[index].width, rects[index].height});
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ExtractBox(const cv::Mat& output, int32_t i, std::vector<int32_t>& box)
    {
        if(output.empty() || i < 0 || i >= output.rows){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        box.clear();
        box.reserve(4);

        // Calculate the scaled coordinates of the bounding box(x, y, width, height)
        box.push_back(static_cast<int32_t>(output.at<float>(i, 0)));
        box.push_back(static_cast<int32_t>(output.at<float>(i, 1)));
        box.push_back(static_cast<int32_t>(output.at<float>(i, 2)));
        box.push_back(static_cast<int32_t>(output.at<float>(i, 3)));

        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ProcessDetectionPredictions(const cv::Mat& results, float confidence_threshold, Predictions& predictions)
    {
        if(results.empty() || confidence_threshold < 0.0 || confidence_threshold > 1.0){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n
        cv::Mat output = Squeeze(results);
        cv::transpose(output, output);

        // Iterate over each row of the output
        double max_score;
        int32_t max_index;
        std::vector<int32_t> box;
        for (int32_t i = 0; i < output.rows; ++i)
        {
            cv::Mat class_scores = output.row(i).colRange(4, output.cols);
            
            // Find the maximum score and its index
            cv::minMaxIdx(class_scores, nullptr, &max_score, nullptr, &max_index);
            // If the maximum score is above the confidence threshold
            if (max_score >= confidence_threshold) {
                // Add the class ID, score, and box coordinates to the respective lists
                if(ExtractBox(output, i, box) != ErrorCode::ERROR_CODE_OK){
                    return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
                }
                predictions.boxes.push_back(box);
                predictions.class_ids.push_back(max_index);
                predictions.scores.push_back(static_cast<float>(max_score));    
            }
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ConstructResultTensor(const Predictions& predictions, std::vector<int>& indices, cv::Mat& output)
    {
        if (predictions.class_ids.empty() || indices.empty()){
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // each row: class_id, score, bbox->(left, top, width, height）
        output.create(std::vector<int32_t>(static_cast<int32_t>(indices.size()), 6), CV_32FC1);

        for (int32_t i = 0; i < indices.size(); ++i) {
            int32_t index = indices[i];
            output.at<float>(i, 0) = static_cast<float>(predictions.class_ids[index]);
            output.at<float>(i, 1) = static_cast<float>(predictions.scores[index]);
            
            const auto& box = predictions.boxes[index];
            output.at<float>(i, 2) = static_cast<float>(box[0]);
            output.at<float>(i, 3) = static_cast<float>(box[1]);
            output.at<float>(i, 4) = static_cast<float>(box[2]);
            output.at<float>(i, 5) = static_cast<float>(box[3]);
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t YoloDetectionPostprocOpencv(const cv::Mat& input, cv::Mat& output, float confidence_threshold)
    {
        std::cout << input.empty() << " " << input.dims << " " << confidence_threshold << std::endl;
        if(input.empty() || confidence_threshold < 0.0 || confidence_threshold > 1.0){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        Predictions predictions;
        if(ProcessDetectionPredictions(input, confidence_threshold, predictions) != ErrorCode::ERROR_CODE_OK){
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if (predictions.class_ids.empty()) {
            output.release();
            return ErrorCode::ERROR_CODE_OK;
        }

        // TODO: 这里的IOU_THRESHOLD需要根据情况调整
        // Apply non-maximum suppression to filter out overlapping bounding boxes
        std::vector<int32_t> indices;
        if(NMSBoxes(predictions.boxes, predictions.scores, confidence_threshold, IOU_THRESHOLD, indices) != ErrorCode::ERROR_CODE_OK){
            output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if(indices.empty()){
            output.release();
            return ErrorCode::ERROR_CODE_OK;
        }

        if(ConstructResultTensor(predictions, indices, output) != ErrorCode::ERROR_CODE_OK){
            output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        return ErrorCode::ERROR_CODE_OK;
    }



    int32_t ProcessSegmentationPredictions(const cv::Mat& results, float confidence_threshold, int32_t num_masks, Predictions& predictions, std::vector<cv::Mat>& mask_predictions)
    {
        if(results.empty() || confidence_threshold < 0.0 || confidence_threshold > 1.0 || num_masks < 1){
            std::cout << "Invalid results" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n, mask_1, mask_2,..., mask_n
        cv::Mat output = Squeeze(results);
        cv::transpose(output, output);
        
        // Total size = bbox (bbox_x, bbox_y, bbox_w, bbox_h) + num_classes + num_masks
        int32_t num_classes = output.cols - num_masks - 4;

        // Iterate over each row of the output
        for (int32_t i = 0; i < output.rows; ++i)
        {
            cv::Mat class_scores = output.row(i).colRange(4, 4 + num_classes);
            double max_score;
            int32_t max_index;
            std::vector<int32_t> box;

            // Find the maximum score and its index
            cv::minMaxIdx(class_scores, nullptr, &max_score, nullptr, &max_index);

            // If the maximum score is above the confidence threshold
            if (max_score >= confidence_threshold) {
                // Add the class ID, score, and box coordinates to the respective lists
                predictions.class_ids.push_back(max_index);
                predictions.scores.push_back(static_cast<float>(max_score));
                if(ExtractBox(output, i, box) != ErrorCode::ERROR_CODE_OK){
                    return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
                }
                predictions.boxes.push_back(box);
                mask_predictions.push_back(output.row(i).colRange(4 + num_classes, output.cols));
            }
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t CropMask(cv::Mat& mask_map, std::vector<std::vector<int32_t>>& scale_boxes, std::vector<std::vector<int32_t>>& boxes,
                    cv::Mat& masks, int32_t input_height, int32_t input_width, int32_t output_height, int32_t output_width) {

        mask_map = cv::Mat::zeros(scale_boxes.size() * input_width * input_height, 1, CV_32F);
        mask_map = mask_map.reshape(1, std::vector<int32_t>{static_cast<int32_t>(scale_boxes.size()), input_width, input_height});

        cv::Size blur_size(int(input_width / output_width), int(input_height / output_height));

        for (size_t i = 0; i < scale_boxes.size(); ++i) {
            int scale_x1 = std::floor(scale_boxes[i][0] - scale_boxes[i][2] / 2);
            int scale_y1 = std::floor(scale_boxes[i][1] - scale_boxes[i][3] / 2);
            int scale_x2 = std::ceil(scale_boxes[i][0] + scale_boxes[i][2] / 2);
            int scale_y2 = std::ceil(scale_boxes[i][1] + scale_boxes[i][3] / 2);

            int x1 = std::floor(boxes[i][0] - boxes[i][2] / 2);
            int y1 = std::floor(boxes[i][1] - boxes[i][3] / 2);
            int x2 = std::ceil(boxes[i][0] + boxes[i][2] / 2);
            int y2 = std::ceil(boxes[i][1] + boxes[i][3] / 2);
 
            // for every box/mask pair, get the mask map
            cv::Mat scale_crop_mask = Squeeze(masks(std::vector<cv::Range>{cv::Range(i, i + 1), cv::Range(scale_y1, scale_y2), cv::Range(scale_x1, scale_x2)}).clone());
            cv::Mat crop_mask;
            cv::resize(scale_crop_mask, crop_mask, cv::Size(x2 - x1, y2 - y1), 0, 0, cv::INTER_CUBIC);
            cv::blur(crop_mask, crop_mask, blur_size);
            cv::threshold(crop_mask, crop_mask, 0.5, 1, cv::THRESH_BINARY);
            mask_map(std::vector<cv::Range>{cv::Range(i, i + 1), cv::Range(y1, y2), cv::Range(x1, x2)}) = crop_mask;
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ProcessSegmentationMaskPredictions(const cv::Mat& results, const cv::Mat& mask_predictions, std::vector<std::vector<int32_t>> boxes, cv::Mat& mask_map)
    {
        if(results.empty() || mask_predictions.empty() || boxes.empty()){
            std::cout << "Invalid arguments" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        cv::Mat output = Squeeze(results);

        // get the CHW shape of the mask output form model
        int32_t num_mask = output.size[0];
        int32_t mask_height = output.size[1];
        int32_t mask_width = output.size[2];

        // nx32 @ 32x(160x160) -> nx(160x160) -> sigmoid -> nx160x160
        output = output.reshape(1, std::vector<int>{num_mask, mask_height * mask_width});
        cv::Mat sigmoid_mask_predictions = ApplySigmoid(mask_predictions * output);
        cv::Mat reshape_sigmoid_mask_predictions = sigmoid_mask_predictions.reshape(1, std::vector<int32_t>{static_cast<int32_t>(sigmoid_mask_predictions.total() / (mask_height * mask_width)), mask_height, mask_width});

        // adjust the bbox coordinates to the mask size
        // TODO: 这里的img_height和img_width需要从算法的信息获取，现在先设定为模型的input尺寸
        int32_t img_height = 1548;
        int32_t img_width = 2656;
        int32_t input_height = 640;
        int32_t input_width = 640;
        std::vector<std::vector<int32_t>> scale_boxes = RescaleBoxes(boxes, input_height, input_width, mask_height, mask_width);

        if(CropMask(mask_map, scale_boxes, boxes, reshape_sigmoid_mask_predictions, 
                        input_height, input_width, mask_height, mask_width) != ErrorCode::ERROR_CODE_OK){
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t YoloSegmentationPostprocOpencv(const cv::Mat& prediction_input, const cv::Mat& mask_input, cv::Mat& prediction_output, cv::Mat& mask_output, float confidence_threshold, int32_t num_masks)
    {
        if(prediction_input.empty() || mask_input.empty() || 
            confidence_threshold < 0.0 || confidence_threshold > 1.0 || num_masks < 1){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        Predictions predictions;
        std::vector<cv::Mat> mask_predictions;
        if(ProcessSegmentationPredictions(prediction_input, confidence_threshold, num_masks, predictions, mask_predictions) != ErrorCode::ERROR_CODE_OK){
            prediction_output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if (predictions.class_ids.empty() || mask_predictions.empty()) {
            prediction_output.release();
            return ErrorCode::ERROR_CODE_OK;
        }

        // TODO: 这里的IOU_THRESHOLD需要根据情况调整
        // Apply non-maximum suppression to filter out overlapping bounding boxes
        std::vector<int32_t> indices;
        if(NMSBoxes(predictions.boxes, predictions.scores, confidence_threshold, IOU_THRESHOLD, indices) != ErrorCode::ERROR_CODE_OK){
            prediction_output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if (indices.empty()) {
            std::cout << "No indices available for mask predictions." << std::endl;
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        cv::Mat mask_predictions_after_nms;

        int totalRows = 0;
        for (auto i : indices) {
            totalRows += mask_predictions[i].rows;
        }

        if (totalRows == 0) {
            std::cout << "Total rows for mask predictions is zero." << std::endl;
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        mask_predictions_after_nms.create(totalRows, mask_predictions[0].cols, mask_predictions[0].type());
        int currentRow = 0;
        for (auto i : indices) {
            if (i >= 0 && i < mask_predictions.size()) {
                mask_predictions[i].copyTo(mask_predictions_after_nms.rowRange(currentRow, currentRow + mask_predictions[i].rows));
                currentRow += mask_predictions[i].rows;
            } else {
                std::cout << "Index out of bounds: " << i << std::endl;
                return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
            }
        }

        if(ProcessSegmentationMaskPredictions(mask_input, mask_predictions_after_nms, predictions.boxes, mask_output) != ErrorCode::ERROR_CODE_OK){
            prediction_output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if(ConstructResultTensor(predictions, indices, prediction_output) != ErrorCode::ERROR_CODE_OK){
            prediction_output.release();
            mask_output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        return ErrorCode::ERROR_CODE_OK;
    }
}