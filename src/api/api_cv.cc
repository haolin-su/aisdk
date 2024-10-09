#include "fibo/api/api_cv.h"

#include "core/model/model.h"
#include "core/graph/graph.h"
#include "core/tensor/tensor.h"
#include "core/registry.h"

#include "common/log.h"

using namespace aisdk;

namespace aisdk {

class CVAPIPrivate {
public:
    // 初始化函数
    int Init(std::string config, std::string model_path);

    // 初始化函数
    int Init(std::string config, const CVAPI_Param &parames, std::string model_path);

    // 获取默认参数函数
    int GetDefaultParam(CVAPI_Param &parames);

    // 释放函数
    int Release();

    // 同步推理函数
    int InferCvSync(const FIBO_CV_Img& img, result::ResultCv& result);

    // 同步推理函数
    int InferCvSync(const std::string& img, result::ResultCv& result);

    // 同步推理函数
    int InferCvClassifySync(const FIBO_CV_Img& img, result::ResultCvClassify& result);

    // 同步推理函数
    int InferCvDetectSync(const FIBO_CV_Img& img, result::ResultCvDetect& result);

    // 同步推理函数
    int InferCvKeypointSync(const FIBO_CV_Img& img, result::ResultCvKeypoint& result);

    // 同步推理函数
    int InferCvSegmentSync(const FIBO_CV_Img& img, result::ResultCvSegment& result);

    // 同步推理函数
    int InferCvFeatureSync(const FIBO_CV_Img& img, result::ResultCvFeature& result);

    // 同步推理函数
    int InferCvSuperResolutionSync(const FIBO_CV_Img& img, result::ResultCvSuperResolution& result);

    // 同步推理函数
    int InferCvRotatedBoxSync(const FIBO_CV_Img& img, result::ResultCvRotatedBox& result);

    // 同步推理函数
    int InferCvOcrSync(const FIBO_CV_Img& img, result::ResultCvOcr& result);

    // 异步推理函数
    int InferAsync(const FIBO_CV_Img& img, CVInferAsyncCallback cb = nullptr, int timeout = 0, const std::string tag = "");

    // 等待推理完成函数
    int WaitTaskDone(const std::string& tag="");

    // 获取模型类型函数
    int GetModelType(std::string &model_type);

    // 获取标签函数
    int GetLabels(std::vector<std::string>& labels);

    // 获取模型信息函数
    int GetModelInfo(FIBO_CV_Model &model_info);

private:
    ModelPtr model_;
    aisdk::GraphPtr graph_ptr_;
};

// 初始化函数
int CVAPIPrivate::Init(std::string config, std::string model_path)
{
    aisdk::SPDLOG::getInstance().init("log.txt", "cv_api", "trace", 1024 * 1024, 3);

    // 注册所有
    aisdk::RegisterAll();

    // 加载模型，也可以在graph创建中加载
    auto model_manager = aisdk::ModelManager::GetInstance();
    model_ = model_manager->LoadModel(model_path);
    if (model_ == nullptr) {
        return -1;
    }

    // 构建graph
    std::shared_ptr<GraphCreator> graph_creator = aisdk::GraphManager::GetInstance()->GetGraphCreator(config);
    if (graph_creator == nullptr) {
        return -1;
    }

    graph_ptr_ = graph_creator->CreateGraph("default_graph", config, model_);
    if (graph_ptr_ == nullptr) {
        return -1;
    }

    return 0;
}

// 初始化函数
int CVAPIPrivate::Init(std::string config, const CVAPI_Param &parames, std::string model_path)
{


    return 0;
}

// 获取默认参数函数
int CVAPIPrivate::GetDefaultParam(CVAPI_Param &parames)
{
    return 0;
}

// 释放函数
int CVAPIPrivate::Release()
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvSync(const FIBO_CV_Img& img, result::ResultCv& result)
{
    // 构建输入tensor
    const TensorShape input_shape = {3, {img.width, img.height, img.channels}};
    const size_t data_size = img.width * img.height * img.channels;
    // Tensor(const std::string& name, const TensorShape& shape, const DataType& dtype, const size_t& data_size, void* data_ptr) 
    TensorPtr input_tensor = std::make_shared<Tensor>("input_tensor", input_shape, DataType::AISDK_DATA_TYPE_UINT8, data_size, (void*)img.data.data());

    // 构建输入meta_data
    MetaDataCvFramePtr meta_data_ptr = std::make_shared<MetaDataCvFrame>();
    // meta_data_ptr->image_path = img;
    meta_data_ptr->image_id = 0;
    meta_data_ptr->original_width = img.width;
    meta_data_ptr->original_height = img.height;
    meta_data_ptr->layout = Layout::AISDK_LAYOUT_NHWC;
    meta_data_ptr->format = img.format;

    input_tensor->meta_data_ptr = meta_data_ptr;

    std::vector<TensorPtr> inputs;
    std::vector<TensorPtr> outputs;
    inputs.emplace_back(input_tensor);

    // 推理
    if (graph_ptr_->Process(inputs, outputs) != 0) {
        return -1;
    }

    // 解析输出tensor
    MetaDataCvFramePtr output_meta_data_ptr = std::static_pointer_cast<MetaDataCvFrame>(outputs[0]->meta_data_ptr);
    aisdk::result::Result result_all = output_meta_data_ptr->result;
    result = result_all.result_cv;

    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvSync(const std::string& img, result::ResultCv& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvClassifySync(const FIBO_CV_Img& img, result::ResultCvClassify& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvDetectSync(const FIBO_CV_Img& img, result::ResultCvDetect& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvKeypointSync(const FIBO_CV_Img& img, result::ResultCvKeypoint& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvSegmentSync(const FIBO_CV_Img& img, result::ResultCvSegment& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvFeatureSync(const FIBO_CV_Img& img, result::ResultCvFeature& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvSuperResolutionSync(const FIBO_CV_Img& img, result::ResultCvSuperResolution& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvRotatedBoxSync(const FIBO_CV_Img& img, result::ResultCvRotatedBox& result)
{
    return 0;
}

// 同步推理函数
int CVAPIPrivate::InferCvOcrSync(const FIBO_CV_Img& img, result::ResultCvOcr& result)
{
    return 0;
}

// 异步推理函数
int CVAPIPrivate::InferAsync(const FIBO_CV_Img& img, CVInferAsyncCallback cb, int timeout, const std::string tag)
{
    return 0;
}


// 等待推理完成函数
int CVAPIPrivate::WaitTaskDone(const std::string& tag)
{
    return 0;
}

// 获取模型类型函数
int CVAPIPrivate::GetModelType(std::string &model_type)
{
    return 0;
}

// 获取标签函数
int CVAPIPrivate::GetLabels(std::vector<std::string>& labels)
{
    return 0;
}

// 获取模型信息函数
int CVAPIPrivate::GetModelInfo(FIBO_CV_Model &model_info)
{
    return 0;
}

}

CVAPI::CVAPI() 
{
    priv_ = new CVAPIPrivate();
}

CVAPI::~CVAPI() 
{
    if (priv_ != nullptr) {
        delete priv_;
        priv_ = nullptr;
    }
}

// 初始化函数
int CVAPI::Init(std::string config, std::string model_path) {
    // Pipeline pipeline;
    // graph1 = pipeline.build_graph(config, model_path);
    // graph2 = pipeline.build_graph(config, model_path);

    // output = graph.Infer(input)

    return priv_->Init(config, model_path);
}

// 初始化函数
int CVAPI::Init(std::string config, const CVAPI_Param &parames, std::string model_path) {

    return 0;
}

// 获取默认参数函数
int CVAPI::GetDefaultParam(CVAPI_Param &parames) {
    return priv_->GetDefaultParam(parames);
}

// 释放函数
int CVAPI::Release() {
    return 0;
}

// 同步推理函数
int CVAPI::InferCvSync(const FIBO_CV_Img& img, result::ResultCv& result) {
    return priv_->InferCvSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvSync(const std::string& img, result::ResultCv& result) {
    return priv_->InferCvSync(img, result);
}
 
// 同步推理函数
int CVAPI::InferCvClassifySync(const FIBO_CV_Img& img, result::ResultCvClassify& result)
{
    return priv_->InferCvClassifySync(img, result);
}

// 同步推理函数
int CVAPI::InferCvDetectSync(const FIBO_CV_Img& img, result::ResultCvDetect& result)
{
    return priv_->InferCvDetectSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvKeypointSync(const FIBO_CV_Img& img, result::ResultCvKeypoint& result)
{
    return priv_->InferCvKeypointSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvSegmentSync(const FIBO_CV_Img& img, result::ResultCvSegment& result)
{
    return priv_->InferCvSegmentSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvFeatureSync(const FIBO_CV_Img& img, result::ResultCvFeature& result)
{
    return priv_->InferCvFeatureSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvSuperResolutionSync(const FIBO_CV_Img& img, result::ResultCvSuperResolution& result)
{
    return priv_->InferCvSuperResolutionSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvRotatedBoxSync(const FIBO_CV_Img& img, result::ResultCvRotatedBox& result)
{
    return priv_->InferCvRotatedBoxSync(img, result);
}

// 同步推理函数
int CVAPI::InferCvOcrSync(const FIBO_CV_Img& img, result::ResultCvOcr& result)
{
    return priv_->InferCvOcrSync(img, result);
}


// 异步推理回调函数
// int CVAPI::CVInferAsyncCallback(std::string& tag, int status, std::string& response) {
//     return 0;
// }

// 异步推理函数
int CVAPI::InferAsync(const FIBO_CV_Img& img, CVInferAsyncCallback cb, int timeout, const std::string tag) {
    return priv_->InferAsync(img, cb, timeout, tag);
}

// 等待推理完成函数
int CVAPI::WaitTaskDone(const std::string& tag) {
    return priv_->WaitTaskDone(tag);
}

// 获取模型类型函数
int CVAPI::GetModelType(std::string &model_type) {
    return priv_->GetModelType(model_type);
}

// 获取标签函数
int CVAPI::GetLabels(std::vector<std::string>& labels) {
    return priv_->GetLabels(labels);
}

// 获取模型信息函数
int CVAPI::GetModelInfo(FIBO_CV_Model &model_info) {
    return priv_->GetModelInfo(model_info);
}
