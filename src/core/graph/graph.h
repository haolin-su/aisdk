#ifndef FIBO_GRAPH_H_
#define FIBO_GRAPH_H_

#include <string>
#include <vector>
#include <memory>

#include "core/model/model.h"
#include "core/tensor/tensor.h"
#include "core/node/node.h"

namespace aisdk {

/* 
graph config file格式：
{
    "name": "graph_name",
    "model": "model_name",
    "version": "1.0.0",
    "inputs": [
        {
            "name": "input_name",
            "shape": [1, 2, 3],
            "dtype": "float32",
            "layout": "NCHW"
        }
    ],
    "outputs": [
        {
            "name": "output_name",
            "shape": [1, 2, 3],
            "dtype": "float32",
            "layout": "NCHW"
        }
    ],
    "nodes": [
        {
            "name": "pic_decode",
            "type": "pic_decode",
            "params": {
                "file_path": "/tmp/test.jpg"
            }
        },
        {
            "name": "preproc",
            "type": "preproc",
            "params": {
                "width": 640,
                "height": 640
            }
        },
        {
            "name": "yolov5",
            "type": "yolov5",
            "params": {
                "model_path": "/tmp/yolov5.onnx"
            }
        },
        {
            "name": "yolov5_postprocess",
            "type": "yolov5_postprocess",
            "params": {
                "conf_threshold": 0.5,
                "iou_threshold": 0.5
            }
        }
    ]
}
*/

// 子图，和下面graph的区别在于这里只能是一个Model，而graph可以是多个Model的subgraph的组合
// class SubGraph {
// public:
//     SubGraph() {}
//     SubGraph(const std::string& name, const ModelPtr& model) : name_(name), model_(model) {}
//     ~SubGraph() {}

//     const std::string& GetName() const { return name_; }
//     const ModelPtr& GetModel() const { return model_; }

//     int Init();
//     int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs);
//     int Finalize();

//     // 获取输入tensor
//     int GetInputTensor(const std::string& name, std::vector<TensorPtr>& tensors);
//     // 获取输出tensor
//     int GetOutputTensor(const std::string& name, std::vector<TensorPtr>& tensors);

//     // 设置输入tensor
//     int SetInputTensor(const std::string& name, const TensorPtr& tensor);
//     // 设置输出tensor
//     int SetOutputTensor(const std::string& name, const TensorPtr& tensor);

//     // 添加节点
//     int AddNode(const std::shared_ptr<INode>& node);
//     // 获取节点
//     int GetNode(const std::string& name, std::shared_ptr<INode>& node);
//     // 获取节点列表
//     int GetNodes(std::vector<std::shared_ptr<INode>>& nodes);


// private:
//     std::string name_;
//     ModelPtr model_;
//     std::vector<TensorPtr> input_tensors_;
//     std::vector<TensorPtr> output_tensors_;
//     std::vector<std::shared_ptr<INode>> nodes_; 
// };
// using SubGraphPtr = std::shared_ptr<SubGraph>;

typedef struct {
    std::string name;
    std::string version;
    std::vector<std::string> model_paths;
    std::vector<Tensor> inputs;
    std::vector<Tensor> outputs;
    std::vector<NodeConfig> nodes;
} GraphConfig;

class Graph {
public:
    Graph() {}
    Graph(const std::string& name) : name_(name) {}
    Graph(const std::string& name, const std::string config_file) : name_(name), config_file_(config_file) {}
    Graph(const std::string& name, const std::string config_file, ModelPtr& model) : name_(name), config_file_(config_file), models_(std::vector<ModelPtr>{model}) {}
    Graph(const std::string& name, const std::string config_file, std::vector<ModelPtr>& models) : name_(name), config_file_(config_file), models_(models) {}
    ~Graph() {}

    const std::string& GetName() const { return name_; }
    const std::string& GetConfigFile() const { return config_file_; }
    std::string GetVersion() const { return version_; }

    int ParseGraphConfig(const std::string& config_file, GraphConfig& config);

    virtual int Init(std::string config_file);
    virtual int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs);
    virtual int Finalize();

    // 获取输入tensor
    virtual int GetInputTensor(const std::string& name, std::vector<TensorPtr>& tensors);
    // 获取输出tensor
    virtual int GetOutputTensor(const std::string& name, std::vector<TensorPtr>& tensors);

    // 添加节点
    int AddNode(const std::shared_ptr<INode>& node);
    // 获取节点
    int GetNode(const std::string& name, std::shared_ptr<INode>& node);
    // 获取节点列表
    int GetNodes(std::vector<std::shared_ptr<INode>>& nodes);

private:
    std::string name_;
    std::vector<ModelPtr> models_;
    std::string version_;
    std::string config_file_;
    std::vector<TensorPtr> input_tensors_;
    std::vector<TensorPtr> output_tensors_;
    std::vector<TensorPtr> tensors_;
    std::vector<std::shared_ptr<INode>> nodes_;
    std::vector<Graph> subgraphs_;
};

using GraphPtr = std::shared_ptr<Graph>;

// 图的创建者模式，创建图，每个图的创建者对象，需要继承这个类，并实现CreateGraph方法
// 为方便后续的解析快速查找graphcreator，name的格式为：graph_name_version，如：yolov5_1.0 
class GraphCreator {
public:
    GraphCreator(const std::string& name) : name_(name) {}
    // Graph(const std::string& name, const std::string config_file, std::vector<ModelPtr>& models) 
    virtual std::shared_ptr<Graph> CreateGraph(const std::string& name, const std::string& config_file, ModelPtr& model)
    {
        return std::make_shared<Graph>(name, config_file, model);
        // return nullptr;
    }
    ~GraphCreator() {}

private:
    std::string name_;
};


// graph的注册管理
// 维护一个注册表，key为graph的名字，唯一标识，value为graph的创造者对象
class GraphManager {
public:
    static std::shared_ptr<GraphManager> GetInstance() {
        if (instance_ == nullptr) {
            instance_ = std::shared_ptr<GraphManager>(new GraphManager());
        }
        return instance_;
    }

    ~GraphManager() {
        graph_creators_.clear();
        // delete instance_;
        // instance_ = nullptr;
    }

    // 注册所有graph
    int RegisterAllGraphsCreator();

    int RegisterGraph(const std::string& name, std::shared_ptr<GraphCreator> creator) {
        graph_creators_[name] = creator;
    }

    std::shared_ptr<GraphCreator> GetGraphCreator(const std::string& name) {
        auto it = graph_creators_.find(name);
        if (it != graph_creators_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    std::unordered_map<std::string, std::shared_ptr<GraphCreator>> graph_creators_;
    static std::shared_ptr<GraphManager> instance_;
};  


}  // namespace aisdk


#endif // FIBO_GRAPH_H_