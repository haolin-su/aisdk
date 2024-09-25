#ifndef FIBO_NLP_LLAMA_PREPROC_H_
#define FIBO_NLP_LLAMA_PREPROC_H_

#include "node.h"

namespace aisdk {

class LlamaPreProcNode : public INode {
public:
    LlamaPreProcNode() = default;
    ~LlamaPreProcNode() = default;
    INode() : name_(""), type_(NODE_TYPE_INPUT) {}
    INode(const std::string& name, NodeType type) : name_(name), type_(type) {}
    ~INode() override {}

    const std::string& GetName() const { return name_; }

    // 初始化
    bool Init() override ;

    // 处理数据
    bool Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) override ;
    bool Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) override ;

    // 释放资源
    bool Finalize() override ;

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
    std::string GetIP() const { return ip_; }
    // 获取版本号
    std::string GetVersion() const { return version_; }

    // 获取硬件IP
    // 获取输入输出tensor
    int GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override ;
    int GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override ;

    // 设置输入输出tensor
    int SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override ;
    int SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override ;

private:
    std::string name_;
    NodeType type_;
    std::string ip_; // 硬件IP加速实现
    std::string version_; // 版本号
    std::unordered_map<std::string, std::string> params_;        // 参数，用于描述node的参数，如输入输出路径，文件名等

    std::vector<std::shared_ptr<INode>> inputs_;   // 输入节点
    std::vector<std::shared_ptr<INode>> outputs_;  // 输出节点
    
}; 
} // namespace aisdk

#endif // FIBO_NLP_LLAMA_PREPROC_H_