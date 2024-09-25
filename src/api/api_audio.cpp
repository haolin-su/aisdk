#include "api/api_audio.h"

using namespace aisdk;

int Audio::Init(std::string &config, std::string& model_path)
{
    return 0;
}


// 释放函数
// 功能说明：释放模型
// 参数说明：
// 返回值：0表示成功，其他表示失败
int Audio::Release()
{
    return 0;
}

// 回调函数
// 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  tag：任务标签，非必须，从中获取值
//  status：任务状态，非必须，从中获取值
//  response：任务响应，非必须，从中获取值
// 返回值：0表示成功，其他表示失败
// int Audio::InferAsyncCallback(std::string& tag, int status, result::nlp::FIBO_Result_Audio_Transalte &response, std::string metrice_str)
// {
//     return 0;
// }


// 音频翻译
// 功能说明：根据输入的wav音频文件进行音频翻译，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  audio 可以传wav 文件路径或音频原始数组, 传文件则在transcribe 加载音频数据,传音频原始数据则由用户给定
//  result 是转录的结果
// 返回值：错误码
int Audio::transcribe(FIBO_Audio audio , aisdk::InferAsyncCallback cb, int timeout, const std::string tag)
{
    return 0;
}

// 获取模型信息
// 功能说明：获取模型信息
// 参数说明：
// 返回值：模型信息结构体
// FIBO_Algorithm Audio::get_model_info()
// {
//     return FIBO_Algorithm();
// }

// 等待任务完成
// 参数说明：
//  tag：任务标签，非必须，可传入""
// 返回值：0表示成功，其他表示失败
int Audio::wait_task_done(const std::string& tag="")
{
    return 0;
}