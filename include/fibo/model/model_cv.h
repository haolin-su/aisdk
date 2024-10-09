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

// 支持的图片格式枚举类型
enum FIBO_CV_Image_Format {
    FIBO_CV_IMAGE_FORMAT_RGB = 0,
    FIBO_CV_IMAGE_FORMAT_BGR,
    FIBO_CV_IMAGE_FORMAT_NV12,
    FIBO_CV_IMAGE_FORMAT_NV21,
    FIBO_CV_IMAGE_FORMAT_YUV420,
    FIBO_CV_IMAGE_FORMAT_MAX
};

typedef struct {
    // 输入图像的最小宽度
    int min_width;
    // 输入图像的最小高度
    int min_height;
    // 输入图像的最大宽度
    int max_width;
    // 输入图像的最大高度
    int max_height;
    // 输入图像的最大通道数
    int max_channels;
    // 输入图像的格式
    FIBO_CV_Image_Format format;
} FIBO_CV_Image_Param;

typedef struct {
    // 输入的图片数量
    int max_batch_size;
    // 输入图像
    FIBO_CV_Image_Param img_param;
} FIBO_CV_PreProc_Param;


// 检测类算法参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // nms阈值，取值范围：[0, 1]
    float nms_threshold = 0.45; 
    // 标签数组
    std::vector<std::string> labels; 
    // anchors数组
    std::vector<float> anchors; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Detect_Param;

// 分类类算法参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Classifier_Param;

// 分割类算法参数结构体
typedef struct {
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Segmentation_Param;

// 姿态类算法参数结构体
typedef struct {
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Keypoint_Param;

// OCR类算法参数结构体
typedef struct {
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_OCR_Param;

// 旋转类算法参数结构体
typedef struct {
    // 置信度阈值，取值范围：[0, 1]
    float threshold = 0.5; 
    // nms阈值，取值范围：[0, 1]
    float nms_threshold = 0.45; 
    // anchors数组
    std::vector<float> anchors; 
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Rotate_Param;

// 超分辨率类算法参数结构体
typedef struct {
    // 标签数组
    std::vector<std::string> labels; 
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Gen_Param;

// 特征提取类算法参数结构体
typedef struct {
    // 模型meta配置，预留字段
    std::string meta_config; 
} FIBO_CV_PostProc_Feature_Param;


typedef struct {
    // 分类算法参数
    FIBO_CV_PostProc_Classifier_Param classifier_param;
    // 检测算法参数
    FIBO_CV_PostProc_Detect_Param detect_param;
    // 分割算法参数
    FIBO_CV_PostProc_Segmentation_Param segmentation_param;
    // 姿态算法参数
    FIBO_CV_PostProc_Keypoint_Param pose_param;
    // OCR算法参数
    FIBO_CV_PostProc_OCR_Param ocr_param;
    // 旋转算法参数
    FIBO_CV_PostProc_Rotate_Param rotate_param;
    // 超分辨率算法参数
    FIBO_CV_PostProc_Gen_Param gen_param;
    // 特征提取算法参数
    FIBO_CV_PostProc_Feature_Param feature_param;
} CVAPI_Param;

enum FIBO_CV_Model_Type {
    FIBO_CV_MODEL_TYPE_CLASSIFY = 0,
    FIBO_CV_MODEL_TYPE_DETECT,
    FIBO_CV_MODEL_TYPE_SEGMENTATION,
    FIBO_CV_MODEL_TYPE_POSE,
    FIBO_CV_MODEL_TYPE_OCR,
    FIBO_CV_MODEL_TYPE_ROTATE,
    FIBO_CV_MODEL_TYPE_GEN,
    FIBO_CV_MODEL_TYPE_FEATURE,
    FIBO_CV_MODEL_TYPE_MAX
};

typedef struct {    
    // 模型名称, 如CV_CLASSIFIER_RESNET50、NLP_SUMMARIZE_YOLOV5等
    std::string name; 
    // 模型类型，如分类、检测、分割、姿态、OCR、旋转、超分辨率、特征提取等
    FIBO_CV_Model_Type type; 
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