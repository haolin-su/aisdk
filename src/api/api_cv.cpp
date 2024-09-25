#include "api/api_cv.h"

using namespace aisdk;

// 初始化函数
// 功能说明：初始化模型，主要为初始化模型路径，加载模型
// 参数说明：
//  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入""
//  model_path：模型路径，必须传入
// 返回值：0表示成功，其他表示失败
int CVAPI::Init(std::string config, std::string model_path) {
    // Pipeline pipeline;
    // graph1 = pipeline.build_graph(config, model_path);
    // graph2 = pipeline.build_graph(config, model_path);

    // output = graph.Infer(input)

    return 0;
}

// 初始化函数
// 功能说明：初始化模型，主要为初始化模型路径，加载模型
// 参数说明：
//  config：配置文件路径，一般为json格式文件，后续制定字段，非必须，可传入""
//  parames：参数结构体，非必须，可传入默认参数
//  model_path：模型路径，必须传入
// 返回值：0表示成功，其他表示失败
int CVAPI::Init(std::string config, const CVAPI_Param &parames, std::string model_path) {

    return 0;
}

// 获取默认参数函数
// 功能说明：获取默认参数
// 参数说明：
// 返回值：默认参数结构体
CVAPI_Param CVAPI::GetDefaultParam() {
    return CVAPI_Param();
}

// 释放函数
// 功能说明：释放模型
// 参数说明：
// 返回值：0表示成功，其他表示失败
int CVAPI::Release() {
    return 0;
}

// 同步推理函数
// 功能说明：同步推理，同步执行，需要等待推理完成
// 参数说明：
//  img：图像结构体，为解码后图片数据，必须传入
//  result：推理结果，空变量，推理完成后，将推理结果填充到result中
// 返回值：0表示成功，其他表示失败
int CVAPI::InferSync(const FIBO_CV_Img& img, result::ResultCv& result) {
    return 0;
}

// 同步推理函数
// 功能说明：同步推理，同步执行，需要等待推理完成
// 参数说明：
//  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
//  result：推理结果，空变量，推理完成后，将推理结果填充到result中
// 返回值：0表示成功，其他表示失败
int CVAPI::InferSync(const std::string& img, result::ResultCv& result) {
    return 0;
}


// 异步推理回调函数
// 功能说明：异步推理回调函数，异步执行，需要配合回调函数和WaitTaskDone函数使用
// 参数说明：
//  tag：任务标签，非必须，从中获取值
//  status：任务状态，非必须，从中获取值
//  response：任务响应，非必须，从中获取值
// 返回值：0表示成功，其他表示失败
// int CVAPI::CVInferAsyncCallback(std::string& tag, int status, std::string& response) {
//     return 0;
// }

// 异步推理函数
// 功能说明：异步推理，异步执行，需要配合回调函数和WaitTaskDone函数使用 
// 参数说明：
//  img：图像结构体，为解码后图片数据，必须传入
//  cb：回调函数，非必须，��传入nullptr
//  timeout：超时时间，单位：毫秒，非必须，可传入0
//  tag：任务标签，非必须
// 返回值：0表示成功，其他表示失败
int CVAPI::InferAsync(const FIBO_CV_Img& img, CVInferAsyncCallback cb, int timeout, const std::string tag) {
    return 0;
}

// 异步推理函数
// 功能说明：异步推理，异步执行，需要配合回调函数和WaitTaskDone函数使用 
// 参数说明：
//  img：图像文件路径，为解码前图片文件，目前支持jpg/png/等opencv可支持的格式，必须传入
//  cb：回调函数，非必须，可传入nullptr
//  timeout：超时时间，单位：毫秒，非必须，可传入0
//  tag：任务标签，非必须
// 返回值：0表示成功，其他表示失败
int CVAPI::InferAsync(const std::string& img, CVInferAsyncCallback cb, int timeout, const std::string tag) {
    return 0;
}

// 等待推理完成函数
// 功能说明：等待推理完成，等待异步推理完成
// 参数说明：
// tag：任务标签，非必须，可传入""
// 返回值：0表示成功，其他表示失败
int CVAPI::WaitTaskDone(const std::string& tag) {
    return 0;
}

// 获取模型类型函数
// 参数说明：
// 返回值：模型类型，比如classify、detect、segment等
std::string CVAPI::GetModelType() {
    return "";
}

// 获取标签函数
// 参数说明：
// 返回值：标签数组
std::vector<std::string> CVAPI::GetLabels() {
    return std::vector<std::string>();
}

// 获取模型信息函数
// 参数说明：
// 返回值：模型信息结构体
FIBO_CV_Model CVAPI::GetModelInfo() {
    return FIBO_CV_Model();
}