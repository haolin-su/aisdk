/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      algorithm.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   algorithm APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_ALGORITHM_ALGORITHM_H_
#define FIBO_ALGORITHM_ALGORITHM_H_

#include <string>
#include <vector>
#include <unordered_map>
#include <memory>

#include "registry.h"
#include "algorithm_desc.h"

namespace aisdk {

// 性能指标结构体
struct FIBO_Algorithm_Performance {
    // 算法名称
    std::string name; 
    // 算法类型
    std::string type; 
    // 算法版本
    std::string version; 
    // 算法时延，单位：微秒，取值范围：[0, 1000000]
    float latency; 
    // 算法内存占用，单位：KB，取值范围：[0, 1000000]
    int memory_usage; 
    // 算法吞吐量，单位：帧/秒，取值范围：[0, 1000000]
    int throughput; 
    // 大模型指标，token速度，单位：token/秒，取值范围：[0, 1000000]
    int token_speed; 
};

// 算法描述结构体
struct AlgorithmDesc {
    // 算法名称
    std::string name; 
    // 算法类型
    std::string type; 
    // 算法版本
    std::string version; 
    // 算法描述
    std::string description; 
    // 算法性能指标
    FIBO_Algorithm_Performance performance; 
};

// 算法类
class Algorithm {
public:
    Algorithm(const AlgorithmDesc& desc) : desc_(desc) {}
    virtual ~Algorithm() = default;

    const AlgorithmDesc& GetDesc() const {
        return desc_;
    }

    virtual bool Init() = 0;
    
    // 模型推理时延
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理时延，单位：毫秒
    float Latency(int warmup_time, int infer_time, int batch_size, int num_samples);

    // 模型推理吞吐量
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理吞吐量，单位：样本/秒
    float Throughput(int warmup_time, int infer_time, int batch_size, int num_samples);

    // 大模型推理速度
    // 参数说明：
    //  warmup_time：预热时间，单位：毫秒
    //  infer_time：模型推理时延，单位：毫秒
    //  batch_size：推理batch大小
    //  num_samples：推理样本数
    // 返回值：模型推理token数/秒
    float Speed(int warmup_time, int infer_time, int batch_size, int num_samples);

private:
    AlgorithmDesc desc_;
};

class AlgorithmFactory {
public:
    static AlgorithmFactory& GetInstance() {
        static AlgorithmFactory instance;
        return instance;
    }

    Algorithm* CreateAlgorithm(const std::string& name) const {
        auto it = algorithms_.find(name);
        if (it!= algorithms_.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    AlgorithmFactory() = default;
    ~AlgorithmFactory() = default;

    std::unordered_map<std::string, std::function<Algorithm*()>> algorithms_;
}

class AlgorithmRegistry {
public:
    static AlgorithmRegistry& GetInstance() {
        static AlgorithmRegistry instance;
        return instance;
    }

    void RegisterAlgorithm(const AlgorithmDesc& desc) {
        algorithms_[desc.name] = std::make_shared<AlgorithmDesc>(desc);
    }

    const AlgorithmDesc* GetAlgorithm(const std::string& name) const {
        auto it = algorithms_.find(name);
        if (it!= algorithms_.end()) {
            return it->second.get();
        }
        return nullptr;
    }

    const std::vector<AlgorithmDesc>& GetAllAlgorithms() const {
        return algorithms_;
    }

private:
    AlgorithmRegistry() = default;
    ~AlgorithmRegistry() = default;

    std::unordered_map<std::string, std::shared_ptr<AlgorithmDesc>> algorithms_;
};

}  // namespace aisdk

#endif  // FIBO_ALGORITHM_ALGORITHM_H_