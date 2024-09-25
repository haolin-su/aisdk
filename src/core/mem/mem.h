#ifndef FIBO_MEM_H_
#define FIBO_MEM_H_

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <mutex>
#include <memory>

namespace aisdk {

// 内存拷贝方向
enum class MemcpyDirection {
    AISDK_MEMCPY_DIR_HOST_TO_DEV,
    AISDK_MEMCPY_DIR_DEV_TO_HOST,
    AISDK_MEMCPY_DIR_HOST_TO_HOST,
    AISDK_MEMCPY_DIR_DEV_TO_DEV
};

// 内存接口
class IMemory {
public:
    // IMemory(){}
    // ~IMemory(){}

    virtual int Alloc(size_t size){ return 0; };
    virtual int Free(void* ptr){ return 0; };
    virtual int DevAlloc(size_t size){ return 0; };
    virtual int DevFree(void* ptr){ return 0; };
    virtual int Memcpy(void* dst, const void* src, size_t size){ return 0; };
    virtual int Memset(void* ptr, int value, size_t size){ return 0; };
    virtual int MemcpyAsync(void* dst, const void* src, size_t size, MemcpyDirection direction){ return 0; };
    virtual int MemsetAsync(void* ptr, int value, size_t size, MemcpyDirection direction){ return 0; };
    virtual int Sync(){ return 0; };

    virtual int Mmap(void* ptr, size_t size){ return 0; };
    virtual int Munmap(void* ptr){ return 0; };

    virtual int GetMemSize(){ return 0; };
    virtual int GetMemPtr(){ return 0; };

private:
    size_t size_;
    void* ptr_; 

    // 保留内存
    void *reserved_ptr_;
};

using IMemoryPtr = std::shared_ptr<IMemory>;

// 内存池
class MemoryPool {
public: 
    MemoryPool();
    ~MemoryPool();

    // 创建内存池
    int Create(size_t size);
    // 销毁内存池
    int Destroy();

private:
    std::mutex mutex_;
    // std::queue<std::shared_ptr<IMemory>> cache_;

    bool created_ = false;

};



// 内存管理类
class MemManager {
public:
    MemManager();
    ~MemManager();

    // static std::shared_ptr<MemManager> GetInstance() {
    //     if (instance_ == nullptr) {
    //         instance_ = std::shared_ptr<MemManager>(new MemManager());
    //     }
    //     return instance_;
    // }

    // // 注册内存
    // // 每个继承IMemory的类都需要注册到内存管理类中，比如每个芯片推理框架的内存管理单元
    // int RegisterMemory(std::shared_ptr<IMemory> memory)
    // {
    //     if (memory == nullptr) {
    //         return -1;
    //     }
    //     memories_[memory->GetName()] = memory;
    //     return 0;
    // }

    // // 获取内存
    // std::shared_ptr<IMemory> GetMemory(const std::string& name)
    // {
    //     auto it = memories_.find(name);
    //     if (it != memories_.end()) {
    //         return it->second;
    //     }
    //     return nullptr;
    // }

    // 获取内存池
    MemoryPool* GetMemoryPool();

private:
    MemoryPool memory_pool_;
    // std::unordered_map<std::string, std::shared_ptr<IMemory>> memories_;
    // std::shared_ptr<IMemory> memory_;
};
}  // namespace aisdk

#endif  // FIBO_MEM_H_