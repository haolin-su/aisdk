/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      api_audio.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   audio APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_API_AUDIO_H_
#define FIBO_API_AUDIO_H_

#include <string>
#include <vector>
#include <functional>
#include "model/model_audio.h"
#include "result.h"

namespace aisdk
{

using AudioInferAsyncCallback = std::function<void(std::string& tag, int status, result::ResultNLP_Audio_Transalte &response, std::string metrice_str)>;

class Audio
{
public:
    // 初始化参数
    // 功能说明：初始化模型，主要为初始化模型路径，加载模型
    // 参数说明：
    // config：配置文件路径，json格式文件，非必须，可传入""，目前不可设置
    // model_path：模型路径，必须传入
    // 返回值：0表示成功，其他表示失败
    int Init(std::string &config, std::string& model_path);


    // 释放函数
    // 功能说明：释放模型
    // 参数说明：
    // 返回值：0表示成功，其他表示失败
    int Release();

    // 回调函数
    // 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  tag：任务标签，非必须，从中获取值
    //  status：任务状态，非必须，从中获取值
    //  response：任务响应，非必须，从中获取值
    // 返回值：0表示成功，其他表示失败
    // int InferAsyncCallback(std::string& tag, int status, result::nlp::ResultNLP_Audio_Transalte &response, std::string metrice_str);


    // 音频转文本同步
    // 功能说明：根据输入的wav音频文件进行音频转文本，同步执行
    // 参数说明：
    //  audio 可以传wav 文件路径或音频原始数组, 传文件则在transcribe 加载音频数据,传音频原始数据则由用户给定
    //  result 是转录的结果
    // 返回值：错误码
    int transcribe(FIBO_Audio audio, result::ResultNLP_Audio_Transalte &result);

    // 音频翻译
    // 功能说明：根据输入的wav音频文件进行音频翻译，异步执行，需要配合回调函数和WaitTaskDone函数使用
    // 参数说明：
    //  audio 可以传wav 文件路径或音频原始数组, 传文件则在transcribe 加载音频数据,传音频原始数据则由用户给定
    //  result 是转录的结果
    // 返回值：错误码
    int transcribe(FIBO_Audio audio , AudioInferAsyncCallback cb, int timeout = 0, const std::string tag = "");

    // 获取模型信息
    // 功能说明：获取模型信息
    // 参数说明：
    // 返回值：模型信息结构体
    // FIBO_Algorithm get_model_info()

    // 等待任务完成
    // 参数说明：
    //  tag：任务标签，非必须，可传入""
    // 返回值：0表示成功，其他表示失败
    int wait_task_done(const std::string& tag);

private:
};

    
} // namespace aisdk

#endif  // FIBO_API_AUDIO_H_
