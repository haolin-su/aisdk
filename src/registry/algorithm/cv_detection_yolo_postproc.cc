#include "cv_detection_yolo_postproc.h"
#include "utils/postproc_opencv.h"
#include "common/tensor_util.h"

using namespace aisdk;
// 初始化
int YoloPostProcNode::Init(std::string config)
{
    std::cout << "YoloPostProcNode Init" << std::endl;
    return true;
}

// 处理数据
int YoloPostProcNode::Process(std::shared_ptr<Tensor>& input, std::shared_ptr<Tensor>& output) 
{
    // input is the tensor from the yolo inferenced results
    // output is the tensor for the postprocessed results
    std::cout << "YoloPostProcNode Process" << std::endl;
    cv::Mat input_mat, output_mat;
    tensor2mat(input, input_mat);
    tensor2mat(output, output_mat);
    YoloDetectionPostprocOpencv(input_mat, output_mat, CONFIDENCE_THRESHOLD);

    return true;
}

int YoloPostProcNode::Process(std::vector<std::shared_ptr<Tensor>>& inputs, std::vector<std::shared_ptr<Tensor>>& outputs) 
{
    // cv::Mat input_mat = cv::Mat(inputs[0]->GetData<float>(), inputs[0]->GetShape());
    // cv::Mat output_mat;
    // YoloPreprocOpencv(input_mat, output_mat, 640, 640);
    // outputs[0]->SetData<float>(output_mat.data, output_mat.total() * output_mat.channels());
    // outputs[0]->SetShape({1, 3, 640, 640});

    return true;
}

// 释放资源
int YoloPostProcNode::Finalize() 
{
    return true;
}

// 获取输入输出节点的数量
int YoloPostProcNode::GetInputSize() 
{
    return 0;
}

int YoloPostProcNode::GetOutputSize() 
{
    return 0;
}

// 设置输入输出节点
int YoloPostProcNode::SetInput(const std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPostProcNode::SetOutput(const std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取输入输出节点
int YoloPostProcNode::GetInput(std::vector<std::shared_ptr<INode>>& inputs) 
{
    return 0;
}

int YoloPostProcNode::GetOutput(std::vector<std::shared_ptr<INode>>& outputs) 
{
    return 0;
}

// 获取参数
int YoloPostProcNode::GetParam(const std::string& key, std::string& value) 
{
    return 0;
}

// 设置参数
int YoloPostProcNode::SetParam(const std::string& key, const std::string& value) 
{
    return 0;
}

// 获取硬件IP
std::string YoloPostProcNode::GetIP() 
{ 
    return ""; 
}

// 获取版本号
std::string YoloPostProcNode::GetVersion() 
{ 
    return ""; 
}

// 获取硬件IP
// 获取输入输出tensor
int YoloPostProcNode::GetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPostProcNode::GetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

// 设置输入输出tensor
int YoloPostProcNode::SetInputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}

int YoloPostProcNode::SetOutputTensor(const std::string& key, std::shared_ptr<Tensor>& tensor) 
{
    return 0;
}