#include "core/model/model.h"

#include "common/log.h"
#include "registry/devices/qualcomm/snpe/snpe_model.h"

using namespace aisdk;

namespace aisdk{
std::shared_ptr<ModelManager> aisdk::ModelManager::instance_ = nullptr;
}

std::shared_ptr<Model> ModelManager::GetModel(const std::string& name) {
    auto it = models_.find(name);
    if (it != models_.end()) {
        return it->second;
    }
    return nullptr;
}

ModelPtr ModelManager::LoadModel(const std::string& model_path)
{
    //TODO: 解密model_path，解析出模型名称和模型路径
    //TODO: 根据模型名称，调用对应的模型创建函数，创建模型对象

    std::string model_name = "snpe_model";

    // Load model
    // auto model = std::make_shared<SnpeModel>(model_path);
    ModelCreatorPtr model_creator = ModelManager::GetModelCreator(model_name);
    if (model_creator == nullptr) {
        return nullptr;
    }  
    ModelPtr model = model_creator->CreateModel(model_path);

    // Register model
    models_[model_name] = model;

    return model;
}

int ModelManager::UnloadModel(const std::string& name)
{
    auto it = models_.find(name);
    if (it != models_.end()) {
        models_.erase(it);
    }

    return 0;
}

int ModelManager::RegisterAllModelCreator()
{
    AISDK_LOG_TRACE("RegisterModelCreatorAll");
    // Register SNPE model creator
    auto snpe_model_creator = std::make_shared<SnpeModelCreator>();
    ModelManager::RegisterModelCreator("snpe_model", snpe_model_creator);

    AISDK_LOG_TRACE("RegisterModelCreatorAll success");
    
    return 0;
}