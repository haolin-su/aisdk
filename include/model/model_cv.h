/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      model_cv.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   CV model APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_MODEL_CV_H_
#define FIBO_MODEL_CV_H_

#include <string>
#include <vector>

namespace aisdk {

// 通用CV模型参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 标签数组
    std::vector<std::string> labels; 
} FIBO_CV_Param;

// 检测类算法参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 最多返回的结果数，取值范围：[1, 1000]
    int top_k = 50; 
    // 保留的结果数，取值范围：[1, 1000]
    int keep_top_k = 50; 
    // nms阈值，取值范围：[0, 1]
    float nms_threshold = 0.45; 
    // 类别数，取值范围：[1, 10000]
    int num_classes = 80; 
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_Detect_Param;

// 分类类算法参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 最多返回的结果数，取值范围：[1, 1000]
    int top_k = 50; 
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_Classifier_Param;

// 分割类算法参数结构体
typedef struct {
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_Segmentation_Param;

// 姿态类算法参数结构体
typedef struct {
    // 输入高度，取值范围：[1, 4096]
    int input_width = 416; 
    // 输入宽度，取值范围：[1, 4096]
    int input_height = 416; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_Pose_Param;


typedef struct {    
    // 模型名称, 如CV_CLASSIFIER_RESNET50、NLP_SUMMARIZE_YOLOV5等
    std::string name; 
    // 模型类型，如yolov5、resnet50等
    std::string type; 
    // 模型版本
    std::string version; 
    // 模型路径
    std::string path; 
    // 模型md5
    std::string md5; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_Model;

}  // namespace aisdk

#endif  // FIBO_MODEL_CV_H_