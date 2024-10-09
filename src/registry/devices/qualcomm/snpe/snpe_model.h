#ifndef SNPE_MODEL_H
#define SNPE_MODEL_H

#include "core/model/model.h"

namespace aisdk {
class SnpeModelPrivate;
class SnpeModel : public Model {
public:
    SnpeModel(){}
    SnpeModel(const std::string& model_path);
    ~SnpeModel();

    virtual int Load(const std::string& model_path) override;
    virtual int Unload() override;

    // 获取模型输入输出的个数
    virtual int GetInputCount() override;
    virtual int GetOutputCount() override;

    // 获取模型输入输出的shape
    virtual std::vector<TensorShape> GetInputShape() override;
    virtual std::vector<TensorShape> GetOutputShape() override;

    // 获取模型输入输出的dtype
    virtual std::vector<DataType> GetInputDtype() override;
    virtual std::vector<DataType> GetOutputDtype() override;

    // 获取模型输入输出的layout
    virtual std::vector<Layout> GetInputLayout() override;
    virtual std::vector<Layout> GetOutputLayout() override;

    // 获取模型输入输出的name
    virtual std::vector<std::string> GetInputName() override;
    virtual std::vector<std::string> GetOutputName() override;

    // 获取模型的输入输出的tensor
    virtual std::vector<TensorPtr> GetInputTensors() override;
    virtual std::vector<TensorPtr> GetOutputTensors() override;\

    // 获取模型的labels
    virtual int GetLabels(std::map<int, std::string>& labels) override;

    // 获取模型的key
    virtual std::string GetKey() override;

    any GetModel() override;
    
private:
    std::shared_ptr<SnpeModelPrivate> priv_;
};

class SnpeModelCreator : public ModelCreator {
public:
    SnpeModelCreator() : ModelCreator("snpe") {}

    ModelPtr CreateModel(const std::string& model_path) {
        return std::make_shared<SnpeModel>(model_path);
    }
};

}

#endif

