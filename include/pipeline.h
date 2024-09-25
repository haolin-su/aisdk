/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      pipeline.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   pipeline APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_PIPELINE_H_
#define FIBO_PIPELINE_H_

#include <string>
#include <vector>
#include <memory>

namespace aisdk {

// 管道类，用于将多个模型串联起来，形成一个完整的推理流程
class Pipeline {
public:
    Pipeline(const std::string& name) : name_(name) {}
    ~Pipeline() {}

    // 初始化管道，加载模型
    virtual bool Init(std::string config_path);
    // 处理输入数据
    virtual bool OnProcess();
    // 释放资源
    virtual bool Finalize();

    const std::string& GetName() const;

protected:
    std::string name_;
};

}  // namespace aisdk

#endif  // FIBO_PIPELINE_H_