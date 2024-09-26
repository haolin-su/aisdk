#include "device.h"

using namespace aisdk;

// 获取CPU使用率
// 参数说明：
// 返回值：CPU资源结构体
int Device::GetCpuUsage(ResourceCpu& cpu) {
    return 0;
}

// 获取GPU使用率
// 参数说明：
// 返回值：GPU资源结构体
int Device::GetGpuUsage(ResourceGpu& gpu) {
    return 0;
}

// 获取DSP使用率
// 参数说明：
// 返回值：DSP资源结构体
int Device::GetDspUsage(ResourceDsp& dsp) {
    return 0;
}

// 获取NPU使用率
// 参数说明：
// 返回值：NPU资源结构体
int Device::GetNpuUsage(ResourceNpu& npu) {
    return 0;
}

// 获取系统内存使用率
// 参数说明：
// 返回值：内存资源结构体
int Device::GetSystemMemoryUsage(ResourceMemory& memory) {
    return 0;
}

// 获取NPU显存使用率
// 参数说明：
// 返回值：显存资源结构体
int Device::GetNpuMemoryUsage(ResourceMemory& memory) {
    return 0;
}