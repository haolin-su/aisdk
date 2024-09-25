#ifndef FIBO_TENSOR_H_
#define FIBO_TENSOR_H_

#include <string>
#include <vector>
#include <memory>

#include "core/mem/mem.h"
#include "core/tensor/meta_data.h"
#include "../utils.h"
// #include "registry.h"

namespace aisdk {


// 张量类，所有tensor的基类
class Tensor {
public:
    Tensor() : name_(""), shape_({}), dtype_(DataType::AISDK_DATA_TYPE_FLOAT32), data_size_(0), data_ptr_(nullptr) {}
    Tensor(const std::string& name) : name_(name) {}
    Tensor(const std::string& name, const TensorShape& shape, const size_t& data_size) : name_(name), shape_(shape), data_size_(data_size), data_ptr_(nullptr) {}
    Tensor(const std::string& name, const TensorShape& shape, const DataType& dtype) : name_(name), shape_(shape), dtype_(dtype), data_size_(0), data_ptr_(nullptr) {}
    Tensor(const std::string& name, const TensorShape& shape, const DataType& dtype, const size_t& data_size) : name_(name), shape_(shape), dtype_(dtype), data_size_(data_size), data_ptr_(nullptr) {}
    Tensor(const std::string& name, const TensorShape& shape, const size_t& data_size, void* data_ptr) : name_(name), shape_(shape), data_size_(data_size), data_ptr_(data_ptr) {}
    Tensor(const std::string& name, const TensorShape& shape, const DataType& dtype, const size_t& data_size, void* data_ptr) : name_(name), shape_(shape), dtype_(dtype), data_size_(data_size), data_ptr_(data_ptr) {}
    
    Tensor(const Tensor& other) : name_(other.name_), shape_(other.shape_), dtype_(other.dtype_), data_size_(other.data_size_), data_ptr_(other.data_ptr_) {}
    virtual ~Tensor() {}

    const std::string& GetName() const { return name_; }

    // 初始化
    virtual int Init();

    // 释放资源
    virtual int Finalize();

    // 获取数据
    virtual int GetData(void* data);
    // 设置数据
    virtual int SetData(void* data);

    // 获取shape
    virtual int GetShape(TensorShape& shape);
    // 设置shape
    virtual int SetShape(const TensorShape& shape);
    
    // 获取数据类型
    virtual int GetDataType(DataType& data_type);
    // 设置数据类型
    virtual int SetDataType(const DataType& data_type);

    // 获取数据大小
    virtual int GetDataSize(size_t& data_size);
    // 设置数据大小
    virtual int SetDataSize(const size_t& data_size);

    // 获取数据指针
    virtual int GetDataPtr(void*& data_ptr);
    // 设置数据指针
    virtual int SetDataPtr(void* data_ptr);

    // 元数据
    MetaDataPtr meta_data_ptr_;

private:
    std::string name_;

    TensorShape shape_;    // shape
    DataType dtype_;       // dtype
    // Layout layout_;        // layout
    size_t data_size_;     // data size
    void* data_ptr_;       // data pointer

    // 内存管理
    IMemoryPtr mem_ptr_;

    // 4个保留位
    uint32_t reserved_[4];
};

using TensorPtr = std::shared_ptr<Tensor>;

// 继承类一般为特定用途的tensor，如input tensor，output tensor，weight tensor等
class InputTensor : public Tensor {
public:
    InputTensor(const std::string& name) : Tensor(name) {}
    virtual ~InputTensor() {}
};

class OutputTensor : public Tensor {
public:
    OutputTensor(const std::string& name) : Tensor(name) {}
    virtual ~OutputTensor() {}
};

class WeightTensor : public Tensor {
public:
    WeightTensor(const std::string& name) : Tensor(name) {}
    virtual ~WeightTensor() {}
};


// 创建tensor的工厂类
class TensorCreator {
public:
    std::shared_ptr<Tensor> CreateTensor(const std::string& name, const TensorType& type);
    ~TensorCreator() {}
};


}  // namespace aisdk

#endif  // FIBO_TENSOR_H_