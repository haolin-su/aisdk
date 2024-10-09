#ifndef FIBO_MODEL_H_
#define FIBO_MODEL_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <condition_variable>

#include "core/utils.h"
#include "core/util/any.h"
#include "core/tensor/tensor.h"

namespace aisdk {

class Model {
public:
    Model() : name_("") {}
    Model(const std::string& name) : name_(name) {}
    virtual ~Model() {}

    const std::string& GetName() const { return name_; }

    virtual int Load(const std::string& model_path) = 0;
    virtual int Unload() = 0;

    // 获取模型输入输出的个数
    virtual int GetInputCount() = 0;
    virtual int GetOutputCount() = 0;

    // 获取模型输入输出的shape
    virtual std::vector<TensorShape> GetInputShape() = 0;
    virtual std::vector<TensorShape> GetOutputShape() = 0;

    // 获取模型输入输出的dtype
    virtual std::vector<DataType> GetInputDtype() = 0;
    virtual std::vector<DataType> GetOutputDtype() = 0;

    // 获取模型输入输出的layout
    virtual std::vector<Layout> GetInputLayout() = 0;
    virtual std::vector<Layout> GetOutputLayout() = 0;

    // 获取模型输入输出的name
    virtual std::vector<std::string> GetInputName() = 0;
    virtual std::vector<std::string> GetOutputName() = 0;

    // 获取模型的输入输出的tensor
    virtual std::vector<TensorPtr> GetInputTensors() = 0;
    virtual std::vector<TensorPtr> GetOutputTensors() = 0;

    virtual int GetLabels(std::map<int, std::string>& labels) = 0;

    // 获取模型的key
    virtual std::string GetKey() = 0;

    virtual any GetModel() = 0;
    
private:
    std::string name_;
};

using ModelPtr = std::shared_ptr<Model>;

class ModelCreator {
public:
    ModelCreator(const std::string& name) : name_(name) {}
    virtual ~ModelCreator() {}

    const std::string& GetName() const { return name_; }

    virtual ModelPtr CreateModel(const std::string& model_path) = 0;
private:
    std::string name_;
};
using ModelCreatorPtr = std::shared_ptr<ModelCreator>;

class ModelManager {
public:
    ModelManager() {}
    ~ModelManager() {
    }

    static std::shared_ptr<ModelManager> GetInstance() {
        if (instance_ == nullptr) {
            instance_ = std::shared_ptr<ModelManager>(new ModelManager());
        }
        return instance_;
    }

    int RegisterAllModelCreator();

    ModelPtr LoadModel(const std::string& model_path);
    int UnloadModel(const std::string& name);

    void RegisterModelCreator(const std::string& name, ModelCreatorPtr creator) {
        creators_[name] = creator;
    }

    ModelCreatorPtr GetModelCreator(const std::string& name) {
        auto it = creators_.find(name);
        if (it != creators_.end()) {
            return it->second;
        }
        return nullptr;
    }

    void RegisterModel(const std::string& md5, std::shared_ptr<Model> model) {
        models_[md5] = model;
    }

    std::shared_ptr<Model> GetModel(const std::string& name);

private:
    static std::shared_ptr<ModelManager> instance_;

    std::unordered_map<std::string, ModelCreatorPtr> creators_;

    // std::mutex mutex_;
    // std::condition_variable cv_;
    std::unordered_map<std::string, std::shared_ptr<Model>> models_;
};

}  // namespace aisdk

#endif