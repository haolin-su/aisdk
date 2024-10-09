/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      api_cv.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   CV APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_API_CV_H_
#define FIBO_API_CV_H_

#include <string>
#include <vector>
#include <functional>

#include "fibo/result.h"
#include "fibo/model/model_cv.h"

namespace aisdk {

// 异步推理回调函数
// 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  tag：任务标签，非必须，从中获取值
//  status：任务状态，非必须，从中获取值
//  response：任务响应，非必须，从中获取值
// 返回值：0表示成功，其他表示失败
using CVInferAsyncCallback = std::function<void(std::string& tag, int status, void* response)>;

typedef struct {
    // 图像宽度，取值范围：[1, 4096]
    int width; 
    // 图像高度，取值范围：[1, 4096]
    int height; 
    // 图像通道数，取值范围：[1, 4]
    int channels; 
    // 图像格式，取值范围：[FIBO_CV_FORMAT_RGB, FIBO_CV_FORMAT_BGR, FIBO_CV_FORMAT_RGBA, FIBO_CV_FORMAT_BGRA, FIBO_CV_FORMAT_GRAY, FIBO_CV_FORMAT_YUV420P，FIBO_CV_FORMAT_NV12, FIBO_CV_FORMAT_NV21]
    int format;
    // 图像布局，取值范围：[FIBO_CV_LAYOUT_HWC, FIBO_CV_LAYOUT_CHW]
    int layout;
    // 图像大小，单位：字节，取值范围：[width * height * channels]
    int size; 
    // 图像数据
    std::vector<uint8_t> data; 
}FIBO_CV_Img;

class CVAPIPrivate;
class CVAPI {
public:
    CVAPI();
    ~CVAPI();

    // 初始化函数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string config, std::string model_path);

    // 初始化函数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入"" 
    //  parames：参数结构体，非必须，可传入默认参数
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string config, const CVAPI_Param &parames, std::string model_path);

    // 获取默认参数函数
    // 功能说明：获取默认参数
    // 参数说明：
    // 返回值：默认参数结构体
    int GetDefaultParam(CVAPI_Param &parames);

    // 释放函数
    // 功能说明：释放模型
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像结构体，为解码后图片数据，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvSync(const FIBO_CV_Img& img, result::ResultCv& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvSync(const std::string& img, result::ResultCv& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvClassifySync(const FIBO_CV_Img& img, result::ResultCvClassify& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvDetectSync(const FIBO_CV_Img& img, result::ResultCvDetect& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvKeypointSync(const FIBO_CV_Img& img, result::ResultCvKeypoint& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvSegmentSync(const FIBO_CV_Img& img, result::ResultCvSegment& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvFeatureSync(const FIBO_CV_Img& img, result::ResultCvFeature& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvSuperResolutionSync(const FIBO_CV_Img& img, result::ResultCvSuperResolution& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvRotatedBoxSync(const FIBO_CV_Img& img, result::ResultCvRotatedBox& result);

    // 同步推理函数
    // 功能说明：同步推理，同步执行，需要等待推理完成
    // 参数说明：
    //  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
    //  result：推理结果，空变量，推理完成后，将推理结果填充到result中
    // 返回值：0表示成功，其他表示失败
    int InferCvOcrSync(const FIBO_CV_Img& img, result::ResultCvOcr& result);

    // 异步推理函数
    // 功能说明：异步推理，异步执行，需要配合回调函数和WaitTaskDone函数使用 
    // 参数说明：
    //  img：图像结构体，为解码后图片数据，必须传入
    //  cb：回调函数，必须传入
    //  timeout：超时时间，单位：毫秒，非必须，传入0
    //  tag：任务标签，非必须，传入""
    // 返回值：0表示成功，其他表示失败
    int InferAsync(const FIBO_CV_Img& img, CVInferAsyncCallback cb = nullptr, int timeout = 0, const std::string tag = "");

    // 等待推理完成函数
    // 功能说明：等待推理完成，等待异步推理完成
    // 参数说明：
    // tag：任务标签，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(const std::string& tag="");

    // 获取模型类型函数
    // 参数说明：
    //  model_type：模型类型，传出参数
    // 返回值：0表示成功，其他表示失败
    int GetModelType(std::string &model_type);

    // 获取标签函数
    // 参数说明：
    //  labels：标签数组，传出参数
    // 返回值：0表示成功，其他表示失败
    int GetLabels(std::vector<std::string>& labels);

    // 获取模型信息函数
    // 参数说明：
    //  model_info：模型信息结构体，传出参数
    // 返回值：0表示成功，其他表示失败
    int GetModelInfo(FIBO_CV_Model &model_info);

private:
    CVAPIPrivate *priv_;
};

} // namespace aisdk

#endif  // FIBO_API_CV_H_