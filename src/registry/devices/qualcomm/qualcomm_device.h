#ifndef FIBO_QUALCOMM_DEVICE_H_
#define FIBO_QUALCOMM_DEVICE_H_

#include "device.h"

namespace aisdk
{

class QualcommDevice : public Device
{
public:
    QualcommDevice();
    ~QualcommDevice();

    // 定义设备的SN号，获取硬件各项资源接口等

private:
};

}

#endif