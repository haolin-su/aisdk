#ifndef FIBO_MEM_H_
#define FIBO_MEM_H_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <mutex>
#include <memory>
#include <unordered_map>

namespace aisdk {

// 内存拷贝方向
enum MemcpyDirection {
    AISDK_MEMCPY_DIR_HOST_TO_DEV,
    AISDK_MEMCPY_DIR_DEV_TO_HOST,
    AISDK_MEMCPY_DIR_HOST_TO_HOST,
    AISDK_MEMCPY_DIR_DEV_TO_DEV
};

enum MemoryType {
    AISDK_MEM_DEVICE_CPU,
    AISDK_MEM_DEVICE_GPU,
    AISDK_MEM_DEVICE_QUALCOMM,
    AISDK_MEM_DEVICE_NVIDIA,
    AISDK_MEM_DEVICE_INTEL,
    AISDK_MEM_DEVICE_OTHER
};

// 内存接口
class Memory {
public:
    // Memory(){};
    // Memory(size_t size) {};
    // virtual  ~Memory(){};

    virtual int Alloc(size_t size) = 0;
    virtual int Free() = 0;
    virtual int DevAlloc(size_t size) = 0;
    virtual int DevFree() = 0;
    virtual int Memcpy(void* dst, const void* src, size_t size) = 0;
    virtual int Memset(void* ptr, int value, size_t size) = 0;
    virtual int MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction) = 0;
    virtual int MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction) = 0;
    virtual int Sync() = 0;

    virtual int Mmap(void* ptr, size_t size) = 0;
    virtual int Munmap(void* ptr) = 0;

    virtual int GetMemSize() = 0;
    virtual void* GetMemPtr() = 0;

    virtual MemoryType GetMemoryType() = 0;
    

private:
    // size_t size_;
    // void* ptr_; 
    // MemoryType mem_type_;

    // // 保留内存
    // void *reserved_ptr_;
};

using MemoryPtr = std::shared_ptr<Memory>;

class MemoryCreator {
public:
    // MemoryCreator(MemoryType type) {}
    // virtual ~MemoryCreator() {}

    virtual std::shared_ptr<Memory> Create(size_t size) = 0;

    virtual MemoryType GetMemoryType(){ return MemoryType::AISDK_MEM_DEVICE_CPU; };

protected:
    // MemoryType type_;
};

// 内存管理类
class MemoryManager {
public:
    MemoryManager() {}
    ~MemoryManager() {}

    static std::shared_ptr<MemoryManager> GetInstance() {
        if (instance_ == nullptr) {
            instance_ = std::shared_ptr<MemoryManager>(new MemoryManager());
        }
        return instance_;
    }

    int RegisterAllMemoryCreators();

    // 注册内存
    // 每个继承IMemory的类都需要注册到内存管理类中，比如每个芯片推理框架的内存管理单元
    int RegisterMemory(std::shared_ptr<MemoryCreator> memory_creator)
    {
        if (memory_creator == nullptr) {
            return -1;
        }
        memory_creators_[memory_creator->GetMemoryType()] = memory_creator;
        return 0;
    }

    // 获取内存
    std::shared_ptr<MemoryCreator> GetMemoryCreator(const MemoryType& type)
    {
        auto it = memory_creators_.find(type);
        if (it != memory_creators_.end()) {
            return it->second;
        }
        return nullptr;
    }

private:
    static std::shared_ptr<MemoryManager> instance_;
    std::unordered_map<MemoryType, std::shared_ptr<MemoryCreator>> memory_creators_;
    std::shared_ptr<Memory> memory_;
};
}  // namespace aisdk

#endif  // FIBO_MEM_H_