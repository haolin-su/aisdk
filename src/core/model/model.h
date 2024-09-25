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
    virtual int GetInputCount(int& input_count) = 0;
    virtual int GetOutputCount(int& output_count) = 0;

    // 获取模型输入输出的shape
    virtual int GetInputShape(std::vector<TensorShape>& input_shapes) = 0;
    virtual int GetOutputShape(std::vector<TensorShape>& output_shapes) = 0;

    // 获取模型输入输出的dtype
    virtual int GetInputDtype(std::vector<DataType>& input_dtypes) = 0;
    virtual int GetOutputDtype(std::vector<DataType>& output_dtypes) = 0;

    // 获取模型输入输出的layout
    virtual int GetInputLayout(std::vector<Layout>& input_layouts) = 0;
    virtual int GetOutputLayout(std::vector<Layout>& output_layouts) = 0;

    // 获取模型输入输出的name
    virtual int GetInputName(std::vector<std::string>& input_names) = 0;
    virtual int GetOutputName(std::vector<std::string>& output_names) = 0;

    // 获取模型的输入输出的tensor
    virtual int GetInputTensor(std::vector<Tensor>& input_tensors) = 0;
    virtual int GetOutputTensor(std::vector<Tensor>& output_tensors) = 0;

    // 获取模型的key
    virtual int GetKey(std::string& key) = 0;

    virtual any GetModel() = 0;
    
private:
    std::string name_;
};

using ModelPtr = std::shared_ptr<Model>;

class ModelManager {
public:
    static ModelManager& GetInstance() {
        static ModelManager instance;
        return instance;
    }

    void RegisterModel(const std::string& md5, std::shared_ptr<Model> model) {
        models_[md5] = model;
    }

    std::shared_ptr<Model> GetModel(const std::string& name) {
        auto it = models_.find(name);
        if (it != models_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<Model>> models_;
};

}  // namespace aisdk

#endif