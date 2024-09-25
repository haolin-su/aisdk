/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      api_multimodal.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   multimodal APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_API_MULTIMODAL_H_
#define FIBO_API_MULTIMODAL_H_

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <chrono>
#include <thread>
#include <functional>

#include "api/api_nlp.h"
#include "api/api_cv.h"

namespace aisdk {

using MultimodalInferAsyncCallback = std::function<void(std::string& tag, int status, result::ResultMultimodal& result)>;

class MultiModalAPI {
public:
    MultiModalAPI() = default;
    virtual ~MultiModalAPI() = default;

// 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  nlp_param：NLP参数结构体，非必须，可传入默认参数
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(const FIBO_NLP_Param& nlp_param, std::string& model_path);

    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    //  config：配置文件路径，json格式文件，非必须，可传入""
    //  model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);


    // 释放函数
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    // tag：任务标签，非必须，从中获取值
    // status：任务状态，非必须，从中获取值
    // response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    // void InferAsyncCallback(std::string& tag, int status, std::string& response);

    // 同步生成接口
    // 功能说明：根据输入的text和img进行图像生成，同步执行
    // 参数说明：
    //  text：输入文本，必须传入
    //  img_path：输入图像路径，非必须，可传入""
    //  result：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, std::string& img_path, result::ResultMultimodal& result);

    // 同步生成接口
    // 功能说明：根据输入的text和img进行图像生成，同步执行
    // 参数说明：
    //  text：输入文本，必须传入
    //  img：输入图像结构体，非必须，可传入""
    //  result：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, const FIBO_CV_Img& img, result::ResultMultimodal& result);

    // 异步生成接口
    // 功能说明：根据输入的text和img进行图像生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  text：输入文本，必须传入
    //  img_path：输入图像路径，非必须，可传入""
    //  cb：回调函数，非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, std::string& img_path, MultimodalInferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 文本生成
    // 功能说明：根据输入的text和img进行图像生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  text：输入文本，必须传入
    //  img：输入图像结构体，非必须，可传入""
    //  cb：回调函数，非必须，可传入nullptr
    //  timeout：超时时间，单位：毫秒，非必须，可传入0
    //  tag：任务标签，非必须
    // 返回值：0表示成功，其他表示失败
    int Generate(std::string& text, const FIBO_CV_Img& img, MultimodalInferAsyncCallback cb, int timeout = 0, std::string tag = "");

    // 获取模型信息
    // 参数说明：
    // 返回值：模型信息结构体
    // FIBO_Algorithm GetModelInfo();

    // 等待任务完成
    // 参数说明：
    //  tag：任务标签，非必须，可传入"" 
    // 返回值：0表示成功，其他表示失败
    int WaitTaskDone(const std::string& tag="");
};

} // namespace aisdk

#endif  // FIBO_API_MULTIMODAL_H_