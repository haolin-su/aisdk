#include "cpu_mem.h"

using namespace aisdk;

#include <memory>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>

#include "common/log.h"

int CPUMemory::Alloc(size_t size)
{
    ptr_ = (void*)malloc(size);
    AISDK_LOG_TRACE("[registry] [cpu] CPUMemory::Alloc size: {}", size);
    return 0;
}
int CPUMemory::Free() 
{
    if (ptr_ != nullptr) {
        free(ptr_);
    }
    AISDK_LOG_TRACE("[registry] [cpu] CPUMemory::Free");
    return 0;
}
int CPUMemory::DevAlloc(size_t size)
{
    return 0;
}
int CPUMemory::DevFree()
{
    return 0;
}
int CPUMemory::Memcpy(void* dst, const void* src, size_t size)
{
    memcpy(dst, src, size);

    return 0;
}
int CPUMemory::Memset(void* ptr, int value, size_t size)
{
    memset(ptr, value, size);

    return 0;
}
int CPUMemory::MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction)
{
    return 0;
}
int CPUMemory::MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction)
{
    return 0;
}
int CPUMemory::Sync()
{
    return 0;
}
int CPUMemory::Mmap(void* ptr, size_t size)
{
    return 0;
}
int CPUMemory::Munmap(void* ptr)
{
    return 0;
}
int CPUMemory::GetMemSize()
{
    return 0;
}
void* CPUMemory::GetMemPtr()
{
    return nullptr;
}

MemoryType CPUMemory::GetMemoryType()
{
    return MemoryType::AISDK_MEM_DEVICE_CPU;
}