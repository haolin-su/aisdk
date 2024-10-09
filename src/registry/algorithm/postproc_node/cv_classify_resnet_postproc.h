#ifndef FIBO_CV_CLASSIFY_RESNET_POSTPROC_H_
#define FIBO_CV_CLASSIFY_RESNET_POSTPROC_H_

#include "core/node/node.h"
#include "core/model/model.h"

namespace aisdk {

class ResnetPostProcNode : public INode {
public:
    ResnetPostProcNode() = default;
    ~ResnetPostProcNode() = default;
    ResnetPostProcNode(const std::string& name) : name_(name) {}
    ResnetPostProcNode(const std::string& name, ModelPtr model) : name_(name), model_(model) {}

    const std::string& GetName() const { return name_; }

    // 初始化
    int Init(std::string config) override ;

    // 处理数据
    int Process(TensorPtr& input, TensorPtr& output) override ;
    int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) override ;

    // 释放资源
    int Finalize() override ;

    // 获取输入输出节点的数量
    int GetInputSize() override ;
    int GetOutputSize() override ;

    // 设置输入输出节点
    int SetInput(const std::vector<std::shared_ptr<INode>>& inputs) override ;
    int SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) override ;

    // 获取输入输出节点
    int GetInput(std::vector<std::shared_ptr<INode>>& inputs) override ;
    int GetOutput(std::vector<std::shared_ptr<INode>>& outputs) override ;

    // 获取参数
    int GetParam(const std::string& key, std::string& value) override ;
    // 设置参数
    int SetParam(const std::string& key, const std::string& value) override ;

    // 获取硬件IP
    std::string GetIP() override ;
    // 获取版本号
    std::string GetVersion() override ;

    // 获取硬件IP
    // 获取输入输出tensor

    std::vector<TensorPtr> GetInputTensors() override;
    std::vector<TensorPtr> GetOutputTensors() override;

    // 设置输入输出tensor
    int SetInputTensors(std::vector<TensorPtr>& tensor) override;
    int SetOutputTensors(std::vector<TensorPtr>& tensor) override;

private:
    std::string name_;
    NodeType type_;
    std::string ip_; // 硬件IP加速实现
    std::string version_; // 版本号
    std::unordered_map<std::string, std::string> params_;        // 参数，用于描述node的参数，如输入输出路径，文件名等

    ModelPtr model_;

    std::vector<std::shared_ptr<INode>> inputs_;   // 输入节点
    std::vector<std::shared_ptr<INode>> outputs_;  // 输出节点
};

class ResnetPostProcNodeCreator : public NodeCreator {
public:
    virtual std::shared_ptr<INode> CreateNode(const std::string& name, const std::string& config_file = "") override
    {
        return std::make_shared<ResnetPostProcNode>(name);
    }
};

// register_preprocess("PostProcess_Resnet_any", ResnetPostProcess, params_str);

} // namespace aisdk

#endif // FIBO_CV_CLASSIFY_RESNET_POSTPROC_H_