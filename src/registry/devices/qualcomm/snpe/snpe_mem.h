#ifndef _SNPE_MEM_H_
#define _SNPE_MEM_H_

#include "core/mem/mem.h"

namespace aisdk
{
    
class SnpeMemory : public IMemory
{
public:
    SnpeMemory();
    ~SnpeMemory();

    int Alloc(size_t size) override;
    int Free(void* ptr) override;
    int DevAlloc(size_t size) override;
    int DevFree(void* ptr) override;
    int Memcpy(void* dst, const void* src, size_t size) override;
    int Memset(void* ptr, int value, size_t size) override;
    int MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction) override;
    int MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction) override;
    int Sync() override;

    int Mmap(void* ptr, size_t size) override;
    int Munmap(void* ptr) override;

    int GetMemSize() override;
    int GetMemPtr() override;
    
};

} // namespace aisdk

#endif // 