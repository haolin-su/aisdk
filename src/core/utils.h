#ifndef FIBO_CORE_UTILS_H_
#define FIBO_CORE_UTILS_H_

#include <string>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
#include <cctype>

namespace aisdk {

enum class TensorType {
    AISDK_TENSOR_TYPE_INPUT,
    AISDK_TENSOR_TYPE_OUTPUT,
    AISDK_TENSOR_TYPE_WEIGHT,
    INTERNAL
};

enum class DataType {
    AISDK_DATA_TYPE_FLOAT32,
    AISDK_DATA_TYPE_FLOAT16,
    AISDK_DATA_TYPE_UINT8,
    AISDK_DATA_TYPE_INT8,
    AISDK_DATA_TYPE_INT16,
    AISDK_DATA_TYPE_UINT16,
    AISDK_DATA_TYPE_INT32,
    AISDK_DATA_TYPE_INT64,
    AISDK_DATA_TYPE_UINT32,
    AISDK_DATA_TYPE_UINT64
};

DataType StringToDataType(const std::string& dtype);


typedef struct {
    int dim;
    std::vector<int> shape;
} TensorShape;


enum class Layout {
    AISDK_LAYOUT_NCHW,
    AISDK_LAYOUT_NHWC,
};

enum class ColorFormat {
    AISDK_COLOR_FORMAT_NV12,
    AISDK_COLOR_FORMAT_NV21,
    AISDK_COLOR_FORMAT_I420,
    AISDK_COLOR_FORMAT_IYUV,
    AISDK_COLOR_FORMAT_YUYV,
    AISDK_COLOR_FORMAT_YVYU,
    AISDK_COLOR_FORMAT_UYVY,
    AISDK_COLOR_FORMAT_VYUY,
    AISDK_COLOR_FORMAT_RGB,
    AISDK_COLOR_FORMAT_BGR,
    AISDK_COLOR_FORMAT_RGBA,
    AISDK_COLOR_FORMAT_BGRA,
    AISDK_COLOR_FORMAT_RGBX,
    AISDK_COLOR_FORMAT_BGRX,
    AISDK_COLOR_FORMAT_XRGB,
    AISDK_COLOR_FORMAT_XBGR,
    
};

} // namespace aisdk

#endif // FIBO_CORE_UTILS_H_