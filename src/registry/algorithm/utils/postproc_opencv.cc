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
        std::cout << "NMSBoxes" << std::endl;

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
        std::cout << "boxes.size(): " << boxes.size() << std::endl;
        cv::dnn::NMSBoxes(rects, scores, confidence_threshold, iou_threshold, indices);

        // accroding to the result.h, the max number of boxes is 32
        std::cout << "indices.size(): " << indices.size() << std::endl;
        int32_t max_boxes = std::min(static_cast<int>(indices.size()), 32);
        indices.resize(max_boxes);
        std::cout << "indices.size(): " << indices.size() << std::endl;

        boxes.clear();
        boxes.reserve(max_boxes);
        for (int32_t i = 0; i < max_boxes; ++i) {
            int32_t index = indices[i];
            boxes.push_back(std::vector<int32_t>{rects[index].x, rects[index].y, rects[index].width, rects[index].height});
        }
        std::cout << "boxes.size(): " << boxes.size() << std::endl;
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ExtractBox(const cv::Mat& output, int32_t i, std::vector<int32_t>& box)
    {
        // std::cout << "ExtractBox" << std::endl;
        if(output.empty() || i < 0 || i >= output.rows){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        box.clear();
        box.reserve(4);

        float x = output.at<float>(i, 0);
        float y = output.at<float>(i, 1);
        float w = output.at<float>(i, 2);
        float h = output.at<float>(i, 3);

        // Calculate the scaled coordinates of the bounding box(left, top, width, height)
        box.push_back(static_cast<int32_t>(x - w / 2));
        box.push_back(static_cast<int32_t>(y - h / 2));
        box.push_back(static_cast<int32_t>(w));
        box.push_back(static_cast<int32_t>(h));
        // TODO: 这里需要根据模型的输入尺寸进行以下的调整
        // box.push_back(static_cast<int32_t>((x - w / 2) * x_factor));
        // box.push_back(static_cast<int32_t>((y - h / 2) * y_factor));
        // box.push_back(static_cast<int32_t>(w * x_factor));
        // box.push_back(static_cast<int32_t>(h * y_factor));

        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ProcessDetectionPredictions(const cv::Mat& results, float confidence_threshold, Predictions& predictions)
    {
        std::cout << "ProcessDetectionPredictions" << std::endl;
        if(!isMatValid(results) || confidence_threshold < 0.0 || confidence_threshold > 1.0){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n
        cv::Mat output = squeeze(results);
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
        std::cout << "ConstructResultTensor" << std::endl;

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
        std::cout << "YoloDetectionPostprocOpencv" << std::endl;
        std::cout << input.empty() << " " << input.dims << " " << confidence_threshold << std::endl;
        if(!isMatValid(input) || confidence_threshold < 0.0 || confidence_threshold > 1.0){
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



    int32_t ProcessSegmentationPredictions(const cv::Mat& results, float confidence_threshold, int32_t num_masks, Predictions& predictions, MaskPredictions& mask_predictions)
    {
        std::cout << "ProcessSegmentationPredictions" << std::endl;
        if(!isMatValid(results) || confidence_threshold < 0.0 || confidence_threshold > 1.0 || num_masks < 1){
            std::cout << "Invalid results" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        // Squeeze and transpose the results tensor
        // Sequence: bbox_x, bbox_y, bbox_w, bbox_h, class_1, class_2,..., class_n, mask_1, mask_2,..., mask_n
        cv::Mat output = squeeze(results);
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
                mask_predictions.mask_predictions.push_back(output.row(i).colRange(4 + num_classes, output.cols));
            }
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t crop_mask(cv::Mat& mask_map, std::vector<cv::Rect>& scale_boxes, std::vector<std::vector<int32_t>>& boxes,
                    cv::Mat& masks, cv::Size img_size, cv::Size mask_size) {

        std::cout << "crop_mask" << std::endl;

        // mask_map = cv::Mat::zeros(scale_boxes.size(), img_size.width, img_size.height, CV_32F);
        mask_map = cv::Mat::zeros(scale_boxes.size() * img_size.width * img_size.height, 1, CV_32F);
        mask_map = mask_map.reshape(1, std::vector<int32_t>{static_cast<int32_t>(scale_boxes.size()), img_size.width, img_size.height});

        std::cout << "mask_map" << std::endl;
        print_mat_shape(mask_map);
        std::cout << "masks" << std::endl;
        print_mat_shape(masks);
        cv::Size blur_size(int(img_size.width / mask_size.width), int(img_size.height / mask_size.height));

        for (size_t i = 0; i < scale_boxes.size(); ++i) {
            int scale_x1 = std::floor(scale_boxes[i].x);
            int scale_y1 = std::floor(scale_boxes[i].y);
            int scale_x2 = std::ceil(scale_boxes[i].x + scale_boxes[i].width);
            int scale_y2 = std::ceil(scale_boxes[i].y + scale_boxes[i].height);

            int x1 = std::floor(boxes[i][0]);
            int y1 = std::floor(boxes[i][1]);
            int x2 = std::ceil(boxes[i][0] + boxes[i][2]);
            int y2 = std::ceil(boxes[i][1] + boxes[i][3]);

            std::cout << "scale[x1, y1, x2, y2] = " << scale_x1 << ", " << scale_y1 << ", " << scale_x2 << ", " << scale_y2  << std::endl;
            std::cout << "[x1, y1, x2, y2] = " << x1 << ", " << y1 << ", " << x2 << ", " << y2  << std::endl;
            
            // for every box/mask pair, get the mask map
            // scale_crop_mask = masks[i][scale_y1:scale_y2, scale_x1:scale_x2]
            cv::Mat scale_crop_mask = masks.row(i)(cv::Range(scale_y1, scale_y2), cv::Range(scale_x1, scale_x2));
            std::cout << "scale_crop_mask" << std::endl;
            print_mat_shape(scale_crop_mask);
            cv::Mat crop_mask;
            cv::resize(scale_crop_mask, crop_mask, cv::Size(x2 - x1, y2 - y1), 0, 0, cv::INTER_CUBIC);
            std::cout << "crop_mask" << std::endl;
            print_mat_shape(crop_mask);
            cv::blur(crop_mask, crop_mask, blur_size);

            cv::threshold(crop_mask, crop_mask, 0.5, 1, cv::THRESH_BINARY);
            mask_map(std::vector<cv::Range>{cv::Range(i, i + 1), cv::Range(y1, y2), cv::Range(x1, x2)}) = crop_mask;
        }
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t ProcessSegmentationMaskPredictions(const cv::Mat& results, const MaskPredictions& mask_predictions, std::vector<std::vector<int32_t>> boxes, std::vector<int32_t>& indices, cv::Mat& mask_map)
    {
        std::cout << "ProcessSegmentationMaskPredictions" << std::endl;
        if(!isMatValid(results) || mask_predictions.mask_predictions.empty() || boxes.empty()){
            std::cout << "Invalid arguments" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        std::vector<cv::Mat> mask_prediction_nms;
        for(auto i : indices){
            mask_prediction_nms.push_back(mask_predictions.mask_predictions[i]);
        }

        cv::Mat output = squeeze(results);

        // get the CHW shape of the mask output form model
        int32_t num_mask = output.size[0];
        int32_t mask_height = output.size[1];
        int32_t mask_width = output.size[2];

        // nx32 @ 32x(160x160) -> nx(160x160) -> sigmoid -> nx160x160
        std::vector<cv::Mat> masks;
        // CHW -> C(height*width)
        output = output.reshape(1, std::vector<int>{num_mask, mask_height * mask_width});
        // prediction_mask @ mask_output
        std::cout << "mask_predictions.mask_predictions.size() = " << mask_predictions.mask_predictions.size() << std::endl;
        // for (const auto& mat : mask_prediction_nms) {
        //     if (masks.empty()) {
        //         masks = mat * output;
        //     } else {
        //         masks += mat * output;
        //     }
        // }

        for (const auto& mat : mask_prediction_nms) {
                masks.push_back(mat * output);
        }

        // apply sigmoid
        cv::Mat sigmoid_mask_predictions = apply_sigmoid(masks);
        // return back to CHW
        cv::Mat reshape_sigmoid_mask_predictions = sigmoid_mask_predictions.reshape(1, std::vector<int32_t>{static_cast<int32_t>(masks.total() / (mask_height * mask_width)), mask_height, mask_width});
        std::cout << "reshape_sigmoid_mask_predictions" << std::endl;
        print_mat_shape(reshape_sigmoid_mask_predictions);
        // adjust the bbox coordinates to the mask size
        std::vector<cv::Rect> rects(boxes.size());
        for (int i = 0; i < boxes.size(); ++i) {
            BoxesVec2Rect(boxes[i], rects[i]);
        }

        // TODO: 这里的img_height和img_width需要从算法的信息获取，现在先设定为模型的input尺寸
        int32_t img_height = 640;
        int32_t img_width = 640;
        std::vector<cv::Rect> scale_boxes = rescale_boxes(rects, cv::Size(img_height, img_width), cv::Size(mask_height, mask_width));

        if(crop_mask(mask_map, scale_boxes, boxes, reshape_sigmoid_mask_predictions, 
                        cv::Size(img_height, img_width), cv::Size(mask_height, mask_width)) != ErrorCode::ERROR_CODE_OK){
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }
        std::cout << "pass 3" << std::endl;
        
        return ErrorCode::ERROR_CODE_OK;
    }

    int32_t YoloSegmentationPostprocOpencv(const cv::Mat& prediction_input, const cv::Mat& mask_input, cv::Mat& prediction_output, cv::Mat& mask_output, float confidence_threshold, int32_t num_masks)
    {
        std::cout << "YoloSegmentationPostprocOpencv" << std::endl;

        if(!isMatValid(prediction_input) || !isMatValid(mask_input) || 
            confidence_threshold < 0.0 || confidence_threshold > 1.0 || num_masks < 1){
            std::cout << "Invalid parameters" << std::endl;
            return ErrorCode::ERROR_CODE_CV_INVALID_PARAM;
        }

        Predictions predictions;
        MaskPredictions mask_predictions;
        if(ProcessSegmentationPredictions(prediction_input, confidence_threshold, num_masks, predictions, mask_predictions) != ErrorCode::ERROR_CODE_OK){
            prediction_output.release();
            return ErrorCode::ERROR_CODE_CV_PROCESS_FAILED;
        }

        if (predictions.class_ids.empty() || mask_predictions.mask_predictions.empty()) {
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

        if(ProcessSegmentationMaskPredictions(mask_input, mask_predictions, predictions.boxes, indices, mask_output) != ErrorCode::ERROR_CODE_OK){
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