/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      device.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   device APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_DEVICE_H_
#define FIBO_DEVICE_H_

#include <string>

namespace aisdk {

typedef struct {
    // CPU核心数
    int core_num; 
    // CPU频率，单位：MHz
    int frequency; 
    // CPU使用率，取值范围：[0, 1]
    float usage; 
} ResourceCpu;

typedef struct {
    // 内存总量，单位：MB
    int total; 
    // 内存使用量，单位：MB
    int used; 
    // 内存剩余量，单位：MB
    int free; 
    // 内存使用率，取值范围：[0, 1]
    float usage; 
} ResourceSystemMemory;

typedef struct {
    // NPU内存总量，单位：MB
    int total; 
    // NPU内存使用量，单位：MB
    int used; 
    // NPU内存剩余量，单位：MB
    int free; 
    // NPU内存使用率，取值范围：[0, 1]
    float usage; 
} ResourceNpuMemory;

typedef struct {
    // GPU名称
    std::string name; 
    // GPU核心数
    int core_num; 
    // GPU频率，单位：MHz
    int frequency; 
    // GPU内存总量，单位：MB
    int total; 
    // GPU内存使用量，单位：MB
    int used; 
    // GPU内存剩余量，单位：MB
    int free; 
    // GPU使用率，取值范围：[0, 1]
    float usage; 
} ResourceGpu;

typedef struct {
    // NPU名称
    std::string name; 
    // NPU核心数
    int core_num; 
    // NPU频率，单位：MHz
    int frequency; 
    // NPU内存信息
    ResourceNpuMemory memory; 
} ResourceNpu;

typedef struct {
    // DSP名称
    std::string name; 
    // DSP核心数
    int core_num; 
    // DSP频率，单位：MHz
    int frequency; 
    // DSP使用率，取值范围：[0, 1]
    float usage; 
} ResourceDsp;

typedef struct {
    // 系统内存信息
    ResourceSystemMemory system; 
    // NPU内存信息
    ResourceNpuMemory npu; 
} ResourceMemory;

typedef struct {
    // 系统名称
    std::string name; 
    // 系统版本
    std::string version; 
} ResourceSystem;

typedef struct {
    // CPU信息
    ResourceCpu cpu; 
    // 内存信息
    ResourceMemory memory; 
    // GPU信息
    ResourceGpu gpu; 
    // NPU信息
    ResourceNpu npu; 
    // 系统信息
    ResourceSystem system; 
} ResourceInfo;

class Device {
public:
    Device() = default;
    ~Device() = default;

    // 获取CPU使用率
    // 参数说明：
    // 返回值：CPU资源结构体
    int GetCpuUsage(ResourceCpu& cpu);

    // 获取GPU使用率
    // 参数说明：
    // 返回值：GPU资源结构体
    int GetGpuUsage(ResourceGpu& gpu);

    // 获取DSP使用率
    // 参数说明：
    // 返回值：DSP资源结构体
    int GetDspUsage(ResourceDsp& dsp);

    // 获取NPU使用率
    // 参数说明：
    // 返回值：NPU资源结构体
    int GetNpuUsage(ResourceNpu& npu);

    // 获取内存使用率
    // 参数说明：
    // 返回值：内存资源结构体
    int GetSystemMemoryUsage(ResourceMemory& memory);

    // 获取显存使用率
    // 参数说明：
    // 返回值：显存资源结构体
    int GetNpuMemoryUsage(ResourceMemory& memory);

private:
};

}  // namespace aisdk

#endif  // FIBO_DEVICE_H_