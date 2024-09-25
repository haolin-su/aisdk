#include "tensor.h"

using namespace aisdk;

// 初始化
int Tensor::Init() {
    // 初始化内存管理
    mem_ptr_ = std::make_shared<IMemory>();

    return 0;
}

// 释放资源
int Tensor::Finalize() {
    return 0;
}

// 获取数据
int Tensor::GetData(void* data) {
    return 0;
}

// 设置数据
int Tensor::SetData(void* data) {
    return 0;
}

// 获取shape
int Tensor::GetShape(TensorShape& shape) {
    shape = shape_;
    return 0;
}

// 设置shape
int Tensor::SetShape(const TensorShape& shape) {
    shape_ = shape;
    return 0;
}

// 获取数据大小
int Tensor::GetDataSize(size_t& data_size) {
    data_size = data_size_;
    return 0;
}

// 设置数据大小
int Tensor::SetDataSize(const size_t& data_size) {
    data_size_ = data_size;
    return 0;
}

// 获取数据指针
int Tensor::GetDataPtr(void*& data_ptr) {
    data_ptr = data_ptr_;
    return 0;
}

// 设置数据指针
int Tensor::SetDataPtr(void* data_ptr) {
    this->data_ptr_ = data_ptr;
    return 0;
}

// 获取数据类型
int Tensor::GetDataType(DataType& data_type) {
    data_type = dtype_;
    return 0;
}

// 设置数据类型
int Tensor::SetDataType(const DataType& data_type) {
    dtype_ = data_type;
    return 0;
}

int Tensor::GetLayout(Layout& layout) {
    layout = layout_;
    return 0;
}

int Tensor::SetLayout(const Layout& layout) {
    layout_ = layout;
    return 0;
}

int Tensor::GetMemory(std::shared_ptr<IMemory>& mem_ptr) {
    mem_ptr = mem_ptr_;
    return 0;
}

int Tensor::SetMemory(const std::shared_ptr<IMemory>& mem_ptr) {
    mem_ptr_ = mem_ptr;
    return 0;
}

namespace aisdk {
    
DataType StringToDataType(const std::string& dtype) 
{
    if (dtype == "float32") {
        return DataType::AISDK_DATA_TYPE_FLOAT32;
    } else if (dtype == "float16") {
        return DataType::AISDK_DATA_TYPE_FLOAT16;
    } else if (dtype == "int8") {
        return DataType::AISDK_DATA_TYPE_INT8;
    } else if (dtype == "int32") {
        return DataType::AISDK_DATA_TYPE_INT32;
    } else if (dtype == "int64") {
        return DataType::AISDK_DATA_TYPE_INT64;
    } else if (dtype == "uint8") {
        return DataType::AISDK_DATA_TYPE_UINT8;
    } else if (dtype == "uint32") {
        return DataType::AISDK_DATA_TYPE_UINT32;
    } else if (dtype == "uint64") {
        return DataType::AISDK_DATA_TYPE_UINT64;
    }
    return DataType::AISDK_DATA_TYPE_FLOAT32;
}

} // namespace aisdk