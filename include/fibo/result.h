/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      result.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   result define 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_RESULT_H_
#define FIBO_RESULT_H_

#include <string>
#include <vector>
namespace aisdk {

namespace result{

// ------------------------------分类的目标结果------------------------------
// 分类的单个目标结果
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
} ResultCvClassifyObject;

// 分类的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvClassifyObject> objects;
} ResultCvClassify;

typedef struct {
    int x;
    int y;
    int w;
    int h;
} Bbox;

// 检测的单个目标结果
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
    // 目标框
    Bbox bbox;
    // 其他信息
    // std::any other_info;
} ResultCvDetectObject;

// 检测的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvDetectObject> objects;
} ResultCvDetect;

// ------------------------------姿态识别的结果------------------------------
typedef struct {
    int x;
    int y;
    float score;
} Point;

// 姿态识别的单个目标
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
    // 目标框
    Bbox bbox;
    // 关键点
    std::vector<Point> keypoints;
} ResultCvKeypointObject;

// 姿态识别的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvKeypointObject> objects;
} ResultCvKeypoint;


// ------------------------------图像分割的结果------------------------------
typedef struct {
    // 图片的宽
    int width;
    // 图片的高
    int height;
    // 图片的大小
    int size;
    // 图片的数据
    std::vector<uint8_t> data;
}SegPlanner;

typedef struct {
    int planner_num;
    std::vector<SegPlanner> planners;
}SegPlanners;

typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<SegPlanners> objects;
} ResultCvSegment;

// ------------------------------特征值------------------------------
// 特征值的单个目标
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
    // 目标框位置
    Bbox bbox;
    // 特征值标签，比如具体识别为哪个人
    std::string feature_abel;
    // 特征值
    std::vector<float> feature;
} ResultCvFeatureObject;

// 特征值的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 特征值数组
    std::vector<ResultCvFeatureObject> objects;
} ResultCvFeature;

// ------------------------------图像超分------------------------------
// 超分的单个目标
typedef struct {
    // 图片的宽
    int width;
    // 图片的高
    int height;
    // 图片的通道数
    int channel;
    // 图片的格式
    int format;
    // 图片的大小
    int size;
    // 图片的数据
    std::vector<uint8_t> data;
} ResultCvSuperResolutionObject;

// 超分的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvSuperResolutionObject> objects;
} ResultCvSuperResolution;

// ------------------------------旋转框结果------------------------------
// 旋转框的单个目标
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
    // 旋转框
    Point bbox[4];
    // 旋转框的角度
    float angle;
} ResultCvRotatedBoxObject;

// 旋转框的结果，包含多个目标
typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvRotatedBoxObject> objects;
} ResultCvRotatedBox;


// ------------------------------OCR识别结果------------------------------
// OCR识别的单个目标
typedef struct {
    // 目标标签
    std::string label;
    // 目标得分
    float score;
    // 目标类别
    int class_id;
    // 目标框
    ResultCvRotatedBoxObject rotated_box;
    // 文本
    std::string text;
} ResultCvOcrObject;

typedef struct {
    // 结果数量, 取值范围：[1, 32]  
    int count; 
    // 目标数组
    std::vector<ResultCvOcrObject> objects;
} ResultCvOcr;


// ------------------------------图像推理结果结构体------------------------------
enum ResultCvType {
    E_ResultCvType_CLASSIFY = 0,
    E_ResultCvType_DETECT = 1,
    E_ResultCvType_KEYPOINT = 2,
    E_ResultCvType_SEGMENT = 3,
    E_ResultCvType_FEATURE = 4,
    E_ResultCvType_GEN = 5,
    E_ResultCvType_ROTATED_BOX = 6,
    E_ResultCvType_OCR = 7,
};

typedef struct {
    // 推理类型
    ResultCvType type;
    // 分类结果
    ResultCvClassify classify;
    // 检测结果
    ResultCvDetect detect;
    // 姿态识别结果
    ResultCvKeypoint pose;
    // 图像分割结果
    ResultCvSegment segment;
    // 特征值结果
    ResultCvFeature feature;
    // 超分结果
    ResultCvSuperResolution super_resolution;
    // 旋转框结果
    ResultCvRotatedBox rotated_box;
    // OCR识别结果
    ResultCvOcr ocr;
} ResultCv;

// ------------------------------文本结果------------------------------
typedef struct {
    // 文本
    std::string text;
} ResultNLP_Text;

// ------------------------------音频翻译结果结构体------------------------------
typedef struct {
    // 翻译token            
    std::vector<std::string> speechokens; 
    // 翻译开始时间，单位：微妙，取值范围：[0, 1000000]
    int64_t speech_start; 
    // 翻译结束时间，单位：微妙，取值范围：[0, 1000000]
    int64_t speech_end; 
    // 翻译置信度，取值范围：[0, 1]
    float speech_confence; 
} ResultNLP_Audio_Transalte;

// ------------------------------多模态结果------------------------------
typedef struct {
    // 图像推理结果
    aisdk::result::ResultCv result_cv;
    // 文本结果
    ResultNLP_Text text;
    // 音频翻译结果
    ResultNLP_Audio_Transalte audioransalte;
} ResultMultimodal;

// ------------------------------总结果结构体------------------------------
typedef struct {
    // 图像推理结果
    aisdk::result::ResultCv result_cv;
    // 文本结果
    aisdk::result::ResultNLP_Text result_nlp_text;
    // 音频翻译结果
    aisdk::result::ResultNLP_Audio_Transalte result_nlp_audioransalte;
    // 多模态结果
    aisdk::result::ResultMultimodal result_multimodal;
} Result;

} // namespace result
} // namespace aisdk

#endif  // FIBO_RESULT_H_