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
    virtual int GetInputCount(int& input_count) override;
    virtual int GetOutputCount(int& output_count) override;

    // 获取模型输入输出的shape
    virtual int GetInputShape(std::vector<TensorShape>& input_shapes) override;
    virtual int GetOutputShape(std::vector<TensorShape>& output_shapes) override;

    // 获取模型输入输出的dtype
    virtual int GetInputDtype(std::vector<DataType>& input_dtypes) override;
    virtual int GetOutputDtype(std::vector<DataType>& output_dtypes) override;

    // 获取模型输入输出的layout
    virtual int GetInputLayout(std::vector<Layout>& input_layouts) override;
    virtual int GetOutputLayout(std::vector<Layout>& output_layouts) override;

    // 获取模型输入输出的name
    virtual int GetInputName(std::vector<std::string>& input_names) override;
    virtual int GetOutputName(std::vector<std::string>& output_names) override;

    // 获取模型的输入输出的tensor
    virtual int GetInputTensor(std::vector<Tensor>& input_tensors) override;
    virtual int GetOutputTensor(std::vector<Tensor>& output_tensors) override;

    // 获取模型的key
    virtual int GetKey(std::string& key) override;

    any GetModel() override;
    
private:
    std::shared_ptr<SnpeModelPrivate> priv_;
};

}

#endif

