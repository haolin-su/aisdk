#ifndef FIBO_META_DATA_H_
#define FIBO_META_DATA_H_

#include <string>
#include <vector>
#include <cstdint>

#include "result.h"

namespace aisdk {

// 寄托于tensor的元数据，主要存放比如图像的宽高、推理后的结果等信息
class MetaData {
public:
    MetaData() = default;
    ~MetaData() = default;

    MetaData(const std::string& name) : name_(name) {}

    aisdk::result::Result result_;
private:
    std::string name_;
    
};  

using MetaDataPtr = std::shared_ptr<MetaData>;

}

#endif