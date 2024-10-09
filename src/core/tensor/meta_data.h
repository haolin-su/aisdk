#ifndef FIBO_META_DATA_H_
#define FIBO_META_DATA_H_

#include <string>
#include <vector>
#include <cstdint>

#include "fibo/result.h"
#include "core/utils.h"

namespace aisdk {

// 寄托于tensor的元数据，主要存放比如图像的宽高、推理后的结果等信息
class MetaData {
public:
    MetaData() = default;
    ~MetaData() = default;

    MetaData(const std::string& name) : name_(name) {}

    // TODO: 性能收集数据，需要修改为Profiler类
    // std::vector<float> performance_data;

private:
    std::string name_;
    
};  

using MetaDataPtr = std::shared_ptr<MetaData>;

class MetaDataCvFrame : public MetaData {
public:
    MetaDataCvFrame() : MetaData("MetaDataCvFrame") {}
    ~MetaDataCvFrame() = default;

    // 图片路径
    std::string image_path;

    // 图片id
    int image_id = 0;

    // 图像的宽高
    uint32_t original_width = 0;
    uint32_t original_height = 0;

    // 图像的布局
    Layout layout = Layout::AISDK_LAYOUT_NHWC;

    // 图像的格式
    int format = 0;

    // 模型推理的结果数据
    aisdk::result::Result result;
};

using MetaDataCvFramePtr = std::shared_ptr<MetaDataCvFrame>;

class MetaDataCv : public MetaData {
public:
    MetaDataCv() : MetaData("MetaDataCv") {}
    ~MetaDataCv() = default;

    // 批量图片数量
    int batch_size = 0;

    // 图片帧数据
    std::vector<MetaDataCvFramePtr> frames;
};

using MetaDataCvPtr = std::shared_ptr<MetaDataCv>;

class MetaDataNLP : public MetaData {
public:
    MetaDataNLP() : MetaData("MetaDataNLP") {}
    ~MetaDataNLP() = default;

    // 原始文本
    std::string original_text;
};

using MetaDataNLPPtr = std::shared_ptr<MetaDataNLP>;

}

#endif