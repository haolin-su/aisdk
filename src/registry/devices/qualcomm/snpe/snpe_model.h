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

    virtual bool Init();

    virtual bool Load(const std::string& model_path);
    virtual bool Unload();

    // 获取模型输入输出的个数
    virtual bool GetInputCount(int& input_count);
    virtual bool GetOutputCount(int& output_count);

    // 获取模型输入输出的shape
    virtual bool GetInputShape(std::vector<TensorShape>& input_shapes);
    virtual bool GetOutputShape(std::vector<TensorShape>& output_shapes);

    // 获取模型输入输出的dtype
    virtual bool GetInputDtype(std::vector<DataType>& input_dtypes);
    virtual bool GetOutputDtype(std::vector<DataType>& output_dtypes);

    // 获取模型输入输出的layout
    virtual bool GetInputLayout(std::vector<Layout>& input_layouts);
    virtual bool GetOutputLayout(std::vector<Layout>& output_layouts);

    // 获取模型输入输出的name
    virtual bool GetInputName(std::vector<std::string>& input_names);
    virtual bool GetOutputName(std::vector<std::string>& output_names);

    // 获取模型的key
    virtual bool GetKey(std::string& key);
    
private:
    std::shared_ptr<SnpeModelPrivate> priv_;
};

}

#endif

