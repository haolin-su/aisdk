#ifndef FIBO_SNPE_INFER_H_
#define FIBO_SNPE_INFER_H_

#include "core/node/node.h"

namespace aisdk {

class SnpeInferPriv;
class SnpeInfer : public INode {
public:
    SnpeInfer();
    SnpeInfer(const std::string& name);
    ~SnpeInfer();

    // 初始化
    int Init(std::string config) override;

    // 处理数据
    int Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) override;
    int Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) override;

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
    int GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override;
    int GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override;

    // 设置输入输出tensor
    int SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override;
    int SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) override;
    
private:
    SnpeInferPriv* priv_;
};

} // namespace aisdk

#endif // FIBO_SNPE_INFER_H_