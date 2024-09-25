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

class Graph {
public:
    Graph() {}
    Graph(const std::string& name, std::string config_file) : name_(name), config_file_(config_file) {}
    ~Graph() {}

    const std::string& GetName() const { return name_; }

    int Init(std::string config_file);
    int Process(std::vector<Tensor>& inputs, std::vector<Tensor>& outputs);
    int Finalize();

    // 获取输入tensor
    int GetInputTensor(const std::string& name, std::vector<Tensor>& tensors);
    // 获取输出tensor
    int GetOutputTensor(const std::string& name, std::vector<Tensor>& tensors);

private:
    std::string name_;
    std::string model_;
    std::string version_;
    std::string config_file_;
    std::vector<Tensor> inputs_;
    std::vector<Tensor> outputs_;
    std::vector<std::shared_ptr<INode>> nodes_;
};

// 图的创建者模式，创建图，每个图的创建者对象，需要继承这个类，并实现CreateGraph方法
// 为方便后续的解析快速查找graphcreator，name的格式为：graph_name_version，如：yolov5_1.0 
class GraphCreator {
public:
    std::shared_ptr<Graph> CreateGraph(const std::string& name, const std::string& config_file);
    ~GraphCreator() {}

private:
    std::string name_;
};


}  // namespace aisdk


#endif // FIBO_GRAPH_H_