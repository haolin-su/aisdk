#ifndef FIBO_TENSOR_PIC_H_
#define FIBO_TENSOR_PIC_H_

#include "tensor.h"
#include "utils.h"

namespace aisdk {


class PicTensor : public Tensor {
public:
    PicTensor(const std::string& name) : Tensor(name) {}
    virtual ~PicTensor() {}

    // 获取layout
    virtual int GetLayout(Layout& layout) = 0;
    // 设置layout
    virtual int SetLayout(const Layout& layout) = 0;


    // 获取颜色格式
    virtual int GetColorFormat(ColorFormat& color_format) = 0;
    // 设置颜色格式
    virtual int SetColorFormat(const ColorFormat& color_format) = 0;

private:
    Layout layout_;        // layout
    ColorFormat color_format_; // color format
};


}

#endif