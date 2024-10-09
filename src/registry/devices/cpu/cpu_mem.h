#ifndef FIBO_CPU_MEM_H_
#define FIBO_CPU_MEM_H_

#include "core/mem/mem.h"

namespace aisdk
{
    
class CPUMemory : public Memory
{
public:
    CPUMemory(){}
    CPUMemory(size_t size) : ptr_(nullptr), size_(size) {}
    ~CPUMemory(){}

    int Alloc(size_t size) override;
    int Free() override;
    int DevAlloc(size_t size) override;
    int DevFree() override;
    int Memcpy(void* dst, const void* src, size_t size) override;
    int Memset(void* ptr, int value, size_t size) override;
    int MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction) override;
    int MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction) override;
    int Sync() override;

    int Mmap(void* ptr, size_t size) override;
    int Munmap(void* ptr) override;

    int GetMemSize() override;
    void* GetMemPtr() override;

    MemoryType GetMemoryType() override;

private:
    void* ptr_;
    size_t size_;
};

class CPUMemoryCreator : public MemoryCreator {
public:

    std::shared_ptr<Memory> Create(size_t size) override
    {
        return std::make_shared<CPUMemory>(size);
    }

    aisdk::MemoryType GetMemoryType() override
    {
        return type_;
    }

private:
    aisdk::MemoryType type_ = aisdk::MemoryType::AISDK_MEM_DEVICE_CPU;
};

} // namespace aisdk

#endif // FIBO_CPU_MEM_H_