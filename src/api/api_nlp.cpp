#include "api/api_nlp.h"

using namespace aisdk;

// 初始化参数
// 功能说明：初始化模型，主要为初始化模型路径，加载模型
// 参数说明：
//  nlp_param：NLP参数结构体，非必须，可传入默认参数
//  model_path：模型路径，必须传入
// 返回值：0表示成功，其他表示失败
int NLPAPI::Init(const FIBO_NLP_Param& nlp_param, std::string& model_path) {
    return 0;
}

// 初始化参数
// 功能说明：初始化模型，主要为初始化模型路径，加载模型
// 参数说明：
//  config：配置文件路径，json格式文件，非必须，可传入""
//  model_path：模型路径，必须传入
// 返回值：0表示成功，其他表示失败
int NLPAPI::Init(std::string &config, std::string& model_path) {
    return 0;
}

// 释放函数
// 功能说明：释放模型
// 参数说明：
// 返回值：0表示成功，其他表示失败
int NLPAPI::Release() {
    return 0;
}

// 回调函数
// 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  tag：任务标签，非必须，从中获取值
//  status：任务状态，非必须，从中获取值
//  response：任务响应，非必须，从中获取值
// 返回值：0表示成功，其他表示失败
// int NLPAPI::InferAsyncCallback(std::string& tag, int status, std::string& response) {
//     return 0;
// }


std::string GenerateSync(std::vector<int>& text, result::ResultNLP_Text result, int timeout, const std::string tag)
{
    return "";
}


// 文本生成
// 功能说明：根据输入的text进行文本生成，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  text：输入文本, 必须传入
//  cb：回调函数, 非必须，可传入nullptr
//  timeout：超时时间，单位：毫秒，非必须，可传入0
//  tag：任务标签，非必须
// 返回值：0表示成功，其他表示失败
int NLPAPI::Generate(std::vector<int>& text, NLPInferAsyncCallback cb, int timeout, const std::string tag) {
    return 0;
}

// 获取模型信息
// 功能说明：获取模型信息
// 参数说明：
// 返回值：模型信息结构体
FIBO_NLP_Model NLPAPI::GetModelInfo() {
    return FIBO_NLP_Model();
}

// 等待任务完成
// 功能说明：等待推理完成，等待异步推理完成
// 参数说明：
//  tag：任务标签，非必须，可传入""
// 返回值：0表示成功，其他表示失败
int NLPAPI::WaitTaskDone(const std::string& tag) {
    return 0;
}