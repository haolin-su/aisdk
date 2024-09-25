#ifndef FIBO_MODEL_H_
#define FIBO_MODEL_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <mutex>
#include <condition_variable>

#include "../utils.h"
namespace aisdk {

class Model {
public:
    Model() : name_("") {}
    Model(const std::string& name) : name_(name) {}
    virtual ~Model() {}

    const std::string& GetName() const { return name_; }

    virtual bool Load(const std::string& model_path) = 0;
    virtual bool Unload() = 0;

    // 获取模型输入输出的个数
    virtual bool GetInputCount(int& input_count) = 0;
    virtual bool GetOutputCount(int& output_count) = 0;

    // 获取模型输入输出的shape
    virtual bool GetInputShape(std::vector<TensorShape>& input_shapes) = 0;
    virtual bool GetOutputShape(std::vector<TensorShape>& output_shapes) = 0;

    // 获取模型输入输出的dtype
    virtual bool GetInputDtype(std::vector<DataType>& input_dtypes) = 0;
    virtual bool GetOutputDtype(std::vector<DataType>& output_dtypes) = 0;

    // 获取模型输入输出的layout
    virtual bool GetInputLayout(std::vector<Layout>& input_layouts) = 0;
    virtual bool GetOutputLayout(std::vector<Layout>& output_layouts) = 0;

    // 获取模型输入输出的name
    virtual bool GetInputName(std::vector<std::string>& input_names) = 0;
    virtual bool GetOutputName(std::vector<std::string>& output_names) = 0;

    // 获取模型的key
    virtual bool GetKey(std::string& key) = 0;
    
private:
    std::string name_;
};

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