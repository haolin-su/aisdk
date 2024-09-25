#include "cv_classify_resnet_postproc.h"
#include "utils/preproc_opencv.h"

using namespace aisdk;
// 初始化
bool ResnetPostProcNode::Init()
{
    return true;
}

// 处理数据
bool ResnetPostProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    if (input == nullptr || output == nullptr)
    {
        return false;
    }

    auto &result = output->meta_data_ptr_->result_;

    float max_prob = 0;
    int max_prob_class = -1;

    // float exp_sum = 0.0f;
    // for (int n = 0; n < num_cls; n++)
    // {
    //     exp_sum += expf(data_ptr[n]);
    // }
    
    // for(int i = 0 ; i < num_cls; i++)
    // {
    //     data_ptr[i] = expf(data_ptr[i]);
    //     if(data_ptr[i] > max_prob) 
    //     {
    //         max_prob = data_ptr[i];
    //         max_prob_class = i;
    //     }
    // }
    // max_prob /= exp_sum;

    // result

    return true;
}
bool ResnetPostProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    return true;
}

// 释放资源
bool ResnetPostProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int ResnetPostProcNode::GetInputSize() 
{
    return 1;
}

int ResnetPostProcNode::GetOutputSize() 
{
    return 1;
}

// 设置输入输出节点
int ResnetPostProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int ResnetPostProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int ResnetPostProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}
int ResnetPostProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int ResnetPostProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}
// 设置参数
int ResnetPostProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string ResnetPostProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string ResnetPostProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int ResnetPostProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int ResnetPostProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int ResnetPostProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int ResnetPostProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}