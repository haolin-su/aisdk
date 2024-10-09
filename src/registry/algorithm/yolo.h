#ifndef FIBO_YOLO_H_
#define FIBO_YOLO_H_

#include "core/graph/graph.h"
#include "core/model/model.h"
#include "core/tensor/tensor.h"

namespace aisdk {

class YOLO : public Graph
{
public:
    YOLO();
    YOLO(const std::string& name, const std::string& config_file, ModelPtr& model);
    YOLO(const std::string& name, const std::string& config_file, std::vector<ModelPtr>& models);

    ~YOLO();
    
    int Init(std::string config_file) override;
    int Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) override;
    int Finalize() override;

    // 获取输入tensor
    int GetInputTensor(const std::string& name, std::vector<TensorPtr>& tensors) override;
    // 获取输出tensor
    int GetOutputTensor(const std::string& name, std::vector<TensorPtr>& tensors) override;

private:
    std::vector<ModelPtr> models_;
    std::vector<TensorPtr> inputs_;
    std::vector<TensorPtr> outputs_;
};

class YOLOGraphCreator : public GraphCreator
{
public:
    YOLOGraphCreator() : GraphCreator("YOLO") {}
    ~YOLOGraphCreator() {}

    std::shared_ptr<Graph> CreateGraph(const std::string& name, const std::string& config_file, ModelPtr& model) override;
    std::shared_ptr<Graph> CreateGraph(const std::string& name, const std::string& config_file, std::vector<ModelPtr>& models) override;
};

} // namespace aisdk

#endif  // FIBO_YOLO_H_