#ifndef FIBO_NODE_H_
#define FIBO_NODE_H_

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>
#include <any>

#include "core/model/model.h"
#include "core/tensor/tensor.h"
#include "core/registry.h"

namespace aisdk {

// node的类型
enum NodeType {
    NODE_TYPE_INPUT,        // 输入节点
    NODE_TYPE_OUTPUT,       // 输出节点
    NODE_TYPE_PREPROCESS,   // 预处理节点
    NODE_TYPE_INFERENCE,    // 推理节点
    NODE_TYPE_POSTPROCESS,  // 后处理节点
};

typedef struct {
    std::string name;
    std::string type;
    std::string version;
    std::string run_ip;
    std::vector<std::string> input_names;
    std::vector<std::string> output_names;
    std::string params;
} NodeConfig;

// node的基类，所有node都需要继承这个类，并实现其方法
class INode {
public:
    INode() : name_("") {}
    INode(const std::string& name) : name_(name) {}
    INode(const std::string& name, ModelPtr model) : name_(name), model_(model) {}
    virtual ~INode() {}

    const std::string& GetName() const { return name_; }

    // 初始化
    virtual int Init(std::string config) = 0;

    // 处理数据
    virtual int Process(TensorPtr& input, TensorPtr& output) = 0;
    virtual int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) = 0;

    // 释放资源
    virtual int Finalize() = 0;

    // 获取输入输出节点的数量
    virtual int GetInputSize() = 0;
    virtual int GetOutputSize() = 0;

    // 设置输入输出节点
    virtual int SetInput(const std::vector<std::shared_ptr<INode>>& inputs) = 0;
    virtual int SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) = 0;

    // 获取输入输出节点
    virtual int GetInput(std::vector<std::shared_ptr<INode>>& inputs) = 0;
    virtual int GetOutput(std::vector<std::shared_ptr<INode>>& outputs) = 0;

    // 获取参数
    virtual int GetParam(const std::string& key, std::string& value) = 0;
    // 设置参数
    virtual int SetParam(const std::string& key, const std::string& value) = 0;

    // 获取硬件IP
    virtual std::string GetIP() = 0;
    // 获取版本号
    virtual std::string GetVersion() = 0;

    // 获取硬件IP
    // 获取输入输出tensor
    virtual std::vector<TensorPtr> GetInputTensors() {
        return model_->GetInputTensors();
    }
    virtual std::vector<TensorPtr> GetOutputTensors() {
        return model_->GetOutputTensors();
    }

    // 设置输入输出tensor
    virtual int SetInputTensors(std::vector<TensorPtr>& tensor) = 0;
    virtual int SetOutputTensors(std::vector<TensorPtr>& tensor) = 0;

    // 获取输入输出节点
    std::vector<std::shared_ptr<INode>> GetInputNodes() {
        return inputs_;
    }
    std::vector<std::shared_ptr<INode>> GetOutputNodes() {
        return outputs_;
    }

private:
    std::string name_;
    // NodeType type_;
    // std::string ip_; // 硬件IP加速实现
    // std::string version_; // 版本号
    // std::unordered_map<std::string, std::string> params_;        // 参数，用于描述node的参数，如输入输出路径，文件名等
    ModelPtr model_;

    std::vector<std::shared_ptr<INode>> inputs_;   // 输入节点
    std::vector<std::shared_ptr<INode>> outputs_;  // 输出节点

    // 这里也有可能有memory，用于运行内部的临时数据
};

using NodePtr = std::shared_ptr<INode>;

// 创造者模式，创建node，每个node的创造者对象，需要继承这个类，并实现CreateNode方法
// 为方便后续的解析快速查找nodecreator，name的格式为：node_type_name_version，如：preprocess_yolov5_1.0 
class NodeCreator {
public:
    virtual std::shared_ptr<INode> CreateNode(const std::string& name, const std::string& config_file = "") = 0;
    virtual ~NodeCreator() {}

private:
    std::string name_;  
};  

// node的注册管理
// 维护一个注册表，key为node的名字，唯一标识，value为node的创造者对象
class NodeManager {
public:
    static NodeManager* GetInstance() {
        if (instance_ == nullptr) {
            instance_ = new NodeManager();
        }
        return instance_;
    }

    ~NodeManager() {
        node_creators_.clear();
        delete instance_;
        instance_ = nullptr;
    }

    // 注册所有node
    int RegisterAllNodesCreator();

    int RegisterNode(const std::string& name, std::shared_ptr<NodeCreator> creator) {
        node_creators_[name] = creator;
        return 0;
    }

    std::shared_ptr<NodeCreator> GetNodeCreator(const std::string& name) {
        auto it = node_creators_.find(name);
        if (it != node_creators_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<NodeCreator>> node_creators_;
    static NodeManager* instance_;
};  

#define REGISTER_NODE_CREATOR(node_name, creator) \
    std::shared_ptr<NodeCreator> creator = std::make_shared<creator>(); \
    NodeManager::GetInstance().RegisterNode(node_name, creator);

}  // namespace aisdk


#endif // FIBO_NODE_H_