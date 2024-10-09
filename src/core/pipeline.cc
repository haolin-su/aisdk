#include "fibo/pipeline.h"

using namespace aisdk;

// Pipeline::Pipeline()
// {
// }

// Pipeline::~Pipeline()
// {
// }

// 初始化管道，加载模型
bool Pipeline::Init(std::string config_path)
{
    // 根据config_path构建graph对象


    return true;
}

// 处理输入数据
bool Pipeline::OnProcess()
{
    return true;
}

// 释放资源
bool Pipeline::Finalize()
{
    return true;
}

const std::string& Pipeline::GetName() const 
{ 
    return name_; 
}