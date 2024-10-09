#include "cv_classify_resnet_postproc.h"

#include "common/log.h"

using namespace aisdk;
// 初始化
int ResnetPostProcNode::Init(std::string config)
{
    return 0;
}

// 处理数据
int ResnetPostProcNode::Process(TensorPtr& input, TensorPtr& output) 
{
    AISDK_LOG_TRACE("[registry] [cpu] ResnetPostProcNode::Process");
    if (input == nullptr || output == nullptr)
    {
        AISDK_LOG_ERROR("[registry] [cpu] ResnetPostProcNode::Process input or output is nullptr");
        return false;
    }

    float *data_ptr = nullptr;
    input->GetData(data_ptr);
    if (data_ptr == nullptr)
    {
        AISDK_LOG_ERROR("[registry] [cpu] ResnetPostProcNode::Process data_ptr is nullptr");
        return -1;
    }

    MetaDataCvFramePtr meta_data_ptr = std::static_pointer_cast<MetaDataCvFrame>(input->meta_data_ptr);

    float max_score = 0;
    int max_score_class = -1;
    int num_cls = 1000;

    float exp_sum = 0.0f;
    for (int n = 0; n < num_cls; n++)
    {
        exp_sum += expf(data_ptr[n]);
    }
    
    for(int i = 0 ; i < num_cls; i++)
    {
        data_ptr[i] = expf(data_ptr[i]);
        if(data_ptr[i] > max_score) 
        {
            max_score = data_ptr[i];
            max_score_class = i;
        }
    }
    max_score /= exp_sum;

    AISDK_LOG_TRACE("[registry] [cpu] ResnetPostProcNode::Process max_score: %f, max_score_class: %d", max_score, max_score_class);

    // 输出结果
    aisdk::result::Result &result = meta_data_ptr->result;
    result.result_cv.classify.count = 1;

    // 获取labels
    ModelPtr model = model_;
    std::map<int, std::string> labels;
    model->GetLabels(labels);

    aisdk::result::ResultCvClassifyObject object;
    object.score = max_score;
    object.class_id = max_score_class;
    object.label = labels[max_score_class];
    result.result_cv.classify.objects.emplace_back(object);

    return 0;
}
int ResnetPostProcNode::Process(std::vector<TensorPtr>& inputs, std::vector<TensorPtr>& outputs) 
{
    return 0;
}

// 释放资源
int ResnetPostProcNode::Finalize() 
{
    return 0;
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
std::vector<TensorPtr> ResnetPostProcNode::GetInputTensors() 
{
    return std::vector<TensorPtr>();
}

std::vector<TensorPtr> ResnetPostProcNode::GetOutputTensors() 
{
    return std::vector<TensorPtr>();
}

// 设置输入输出tensor
int ResnetPostProcNode::SetInputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}

int ResnetPostProcNode::SetOutputTensors(std::vector<TensorPtr>& tensor) 
{
    return 0;
}