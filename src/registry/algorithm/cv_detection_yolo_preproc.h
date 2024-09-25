#ifndef FIBO_CV_DETECTION_YOLO_PREPROC_H_
#define FIBO_CV_DETECTION_YOLO_PREPROC_H_

#include "core/node/node.h"
#include "core/node/node.h"

namespace aisdk {

class YoloPreProcNode : public INode {
public:
    YoloPreProcNode() = default;
    ~YoloPreProcNode() = default;
    // INode() : INode(""), type_(NODE_TYPE_INPUT) {}
    YoloPreProcNode(const std::string& name) : INode(name) {}

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
    std::string GetIP() override ;
    // 获取版本号
    std::string GetVersion() override ;

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

// register_preprocess("PreProcess_Yolo_any", YoloPreProcess, params_str);

} // namespace aisdk

#endif // FIBO_CV_DETECTION_YOLO_PREPROC_H_