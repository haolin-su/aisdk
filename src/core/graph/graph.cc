#include "graph.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <stdexcept>
#include "nlohmann/json.hpp"

using json = nlohmann::json;

using namespace aisdk;

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
    ],
    "tensors": [
        {
            "name": "tensor_name",
            "shape": [1, 2, 3],
            "dtype": "float32",
            "layout": "NCHW"
        }
    ]
}
*/

namespace aisdk {
std::shared_ptr<GraphManager> GraphManager::instance_ = nullptr;

}

int Graph::ParseGraphConfig(const std::string& config_file, GraphConfig& config) {
    try {
        // 打开并读取配置文件
        std::ifstream file(config_file);
        if (!file.is_open()) {
            return -1;
        }

        json config_json;
        file >> config_json;

        // 解析基本信息
        config.name = config_json.at("name").get<std::string>();
        // model_ = config_json.at("model").get<std::string>();
        // TODO: 解析model再load model
        config.version = config_json.at("version").get<std::string>();

        // 解析输入
        for (const auto& input : config_json["inputs"]) {
            TensorShape shape;
            shape.dim = input.at("shape").size();
            shape.shape = input.at("shape").get<std::vector<int>>();

            // Layout layout = Layout::NCHW;
            // if (input.at("layout").is_string()) {
            //     layout = static_cast<Layout>(input.at("layout").get<std::string>());
            // }

            DataType dtype = DataType::AISDK_DATA_TYPE_FLOAT32;
            if (input.at("dtype").is_string()) {
                dtype = StringToDataType(input.at("dtype").get<std::string>());
            }

            Tensor it(input.at("name").get<std::string>(), shape, dtype);
            config.inputs.emplace_back(it);
        }

        // 解析输出
        for (const auto& output : config_json["outputs"]) {
            TensorShape shape;  
            shape.dim = output.at("shape").size();
            shape.shape = output.at("shape").get<std::vector<int>>();

            DataType dtype = DataType::AISDK_DATA_TYPE_FLOAT32;
            if (output.at("dtype").is_string()) {
                dtype = StringToDataType(output.at("dtype").get<std::string>());
            }

            Tensor ot(output.at("name").get<std::string>(), shape, dtype);
            config.outputs.emplace_back(ot);
        }

        // 解析节点
        for (const auto& node : config_json["nodes"]) {
            

            // std::shared_ptr<INode> nd = creator->CreateNode(node.at("name").get<std::string>(), node.at("params").get<json>());
            // nd->SetParam("params", node.at("params").get<json>());
            NodeConfig node_config;
            node_config.name = node.at("name").get<std::string>();
            node_config.type = node.at("type").get<std::string>();
            node_config.params = node.at("params").get<std::string>();
            config.nodes.emplace_back(node_config);
        }

        file.close();
    } catch (const json::exception& e) {
        return -1;
    } catch (const std::exception& e) {
        return -1;
    }

    return 0;
}

int Graph::Init(std::string config_file) {
    std::vector<Tensor> inputs;
    std::vector<Tensor> outputs;
    std::vector<std::shared_ptr<INode>> nodes;
    std::vector<Tensor> tensors;

    std::vector<std::string> model_paths;

    GraphConfig config;

    // 解析配置文件
    ParseGraphConfig(config_file, config);

    // TODO: 初始化nodes
    // 遍历所有节点，初始化节点
    NodeManager* nm = NodeManager::GetInstance();
    
    for (const auto& node : config.nodes) {
        std::shared_ptr<NodeCreator> creator = nm->GetNodeCreator(node.type);
        if (creator == nullptr) {
            return -1;
        }
        // std::shared_ptr<INode> nd = creator->CreateNode(node.at("name").get<std::string>(), node.at("params").get<json>());
        // nd->SetParam("params", node.at("params").get<json>());
        std::shared_ptr<INode> nd = creator->CreateNode(config.name, "");
        nd->Init(node.params);
        nodes_.emplace_back(nd);
    }

    // TODO: 初始化tensors
    // 遍历多有的nodes，获取其inputs和outputs，初始化tensors
    for (const auto& node : nodes_) {
        std::vector<TensorPtr> inputs = node->GetInputTensors();
        std::vector<TensorPtr> outputs = node->GetOutputTensors();

        for (const auto& input : inputs) {
            if (std::find(tensors_.begin(), tensors_.end(), input) == tensors_.end()) {
                if (false == input->IsData())
                {
                    input->MallocData();
                }
                tensors_.emplace_back(input);
            }
        }

        for (const auto& output : outputs) {
            if (std::find(tensors_.begin(), tensors_.end(), output) == tensors_.end()) {
                tensors_.emplace_back(output);
            }
        }
    }

    return 0;
}

int Graph::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) {
    // TODO: 改为多个子图串并联分发运行等多种策略来执行计算图
    std::vector<TensorPtr> tmp;

    // 1. 遍历所有节点，执行节点处理
    for (const auto& node : nodes_) {
        if (tmp.size() != 0) {
            inputs = tmp;
        }

        // TODO: outputs需要重新分配内存，最后一个节点才使用outputs
        node->Process(inputs, outputs);
        tmp = outputs;
    }

    return 0;
}

int Graph::Finalize() {
    // 

    return 0;
}

int Graph::GetInputTensor(const std::string& name, std::vector<TensorPtr>& tensors) {
    return 0;
}

int Graph::GetOutputTensor(const std::string& name, std::vector<TensorPtr>& tensors) {
    return 0;
}

// 添加节点
int Graph::AddNode(const std::shared_ptr<INode>& node)
{
    return 0;
}

// 获取节点
int Graph::GetNode(const std::string& name, std::shared_ptr<INode>& node)
{
    return 0;
}

// 获取节点列表
int Graph::GetNodes(std::vector<std::shared_ptr<INode>>& nodes)
{
    return 0;
}



int GraphManager::RegisterAllGraphsCreator()
{
    return 0;
}
