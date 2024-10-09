#ifndef FIBO_MEM_POOL_H_
#define FIBO_MEM_POOL_H_

#include <cstddef>
#include <mutex>

namespace aisdk {

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
    // std::queue<std::shared_ptr<Memory>> cache_;

    bool created_ = false;

};
} // namespace aisdk

#endif // FIBO_MEM_POOL_H_