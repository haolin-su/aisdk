#ifndef FIBO_SNPE_INFER_H_
#define FIBO_SNPE_INFER_H_

#include "core/node/node.h"

namespace aisdk {

class SnpeInferPriv;
class SnpeInferNode : public INode {
public:
    SnpeInferNode();
    SnpeInferNode(const std::string& name);
    SnpeInferNode(const std::string& name, ModelPtr model);
    ~SnpeInferNode();

    // 初始化
    int Init(std::string config) override;

    // 处理数据
    int Process(TensorPtr& input, TensorPtr& output) override;
    int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) override;

    // 释放资源
    int Finalize() override;

    // 获取输入输出节点的数量
    int GetInputSize() override;
    int GetOutputSize() override;

    // 设置输入输出节点
    int SetInput(const std::vector<std::shared_ptr<INode>>& inputs) override;
    int SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) override;

    // 获取输入输出节点
    int GetInput(std::vector<std::shared_ptr<INode>>& inputs) override;
    int GetOutput(std::vector<std::shared_ptr<INode>>& outputs) override;

    // 获取参数
    int GetParam(const std::string& key, std::string& value) override;
    // 设置参数
    int SetParam(const std::string& key, const std::string& value) override;

    // 获取硬件IP
    std::string GetIP() override;
    // 获取版本号
    std::string GetVersion() override;

    // 获取硬件IP
    // 获取输入输出tensor
    // std::vector<TensorPtr> GetInputTensors() override;
    // std::vector<TensorPtr> GetOutputTensors() override;

    // 设置输入输出tensor
    int SetInputTensors(std::vector<TensorPtr>& tensor) override;
    int SetOutputTensors(std::vector<TensorPtr>& tensor) override;
    
private:
    std::shared_ptr<SnpeInferPriv> priv_;
};

class SnpeInferNodeCreator : public NodeCreator {
public:
    virtual std::shared_ptr<INode> CreateNode(const std::string& name, const std::string& config_file = "") override
    {
        return std::make_shared<SnpeInferNode>(name);
    }
};


} // namespace aisdk

#endif // FIBO_SNPE_INFER_H_