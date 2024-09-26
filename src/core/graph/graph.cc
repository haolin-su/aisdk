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

int Graph::Init(std::string config_file) {
    try {
        // 打开并读取配置文件
        std::ifstream file(config_file);
        if (!file.is_open()) {
            return -1;
        }

        json config_json;
        file >> config_json;

        // 解析基本信息
        name_ = config_json.at("name").get<std::string>();
        model_ = config_json.at("model").get<std::string>();
        version_ = config_json.at("version").get<std::string>();

        // 解析输入
        std::vector<Tensor> inputs;
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
            inputs.emplace_back(it);
        }

        // 解析输出
        std::vector<Tensor> outputs;
        for (const auto& output : config_json["outputs"]) {
            TensorShape shape;  
            shape.dim = output.at("shape").size();
            shape.shape = output.at("shape").get<std::vector<int>>();

            DataType dtype = DataType::AISDK_DATA_TYPE_FLOAT32;
            if (output.at("dtype").is_string()) {
                dtype = StringToDataType(output.at("dtype").get<std::string>());
            }

            Tensor ot(output.at("name").get<std::string>(), shape, dtype);
            outputs.emplace_back(ot);
        }
        // 解析节点
        std::vector<std::shared_ptr<INode>> nodes;
        for (const auto& node : config_json["nodes"]) {
            NodeManager& nm = NodeManager::GetInstance();
            std::shared_ptr<NodeCreator> creator = nm.GetNodeCreator(node.at("type").get<std::string>());
            if (creator == nullptr) {
                return -1;
            }

            std::shared_ptr<INode> nd = creator->CreateNode(node.at("name").get<std::string>(), node.at("params").get<json>());
            nd->SetParam("params", node.at("params").get<json>());
            nodes.emplace_back(nd);
        }

        file.close();
        return 0;
    } catch (const json::exception& e) {
        return -1;
    } catch (const std::exception& e) {
        return -1;
    }
}

int Graph::Process(std::vector<Tensor>& inputs, std::vector<Tensor>& outputs) {
    // 1. 遍历所有节点，执行节点处理
    // for (const auto& node : nodes_) {
    //     node->Process(inputs, outputs);
    // }

    return 0;
}

int Graph::Finalize() {
    return 0;
}

int Graph::GetInputTensor(const std::string& name, std::vector<Tensor>& tensors) {
    return 0;
}

int Graph::GetOutputTensor(const std::string& name, std::vector<Tensor>& tensors) {
    return 0;
}