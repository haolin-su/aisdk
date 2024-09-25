/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      model_audio.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   audio model APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_MODEL_AUDIO_H_
#define FIBO_MODEL_AUDIO_H_

#include <string>
#include <vector>

namespace aisdk {

typedef struct {
    // 音频采样率，取值范围：[8000, 48000]
    int32_t audio_sample_rate; 
    // 音频通道数，取值范围：[1, 2]
    int32_t audio_channel; 
    // 音频存储格式，取值范围：[FIBO_AUDIO_FORMAT_PCM, FIBO_AUDIO_FORMAT_MP3, FIBO_AUDIO_FORMAT_AAC, FIBO_AUDIO_FORMAT_WAV]     
    int32_t audio_format; 
    // 音频chunk 大小，取值范围：[1, 4096]
    int32_t audio_buffer_bytes_size; 
    // 音频时长，单位：秒，取值范围：[0, 1000]
    int32_t audio_duration; 
    // 音频原始数据引用
    std::vector<uint8_t> &audio_buffer;
} FIBO_Audio;

typedef struct {
    // 模型名称
    std::string model_name;
    // 模型版本
    std::string model_version;
    // 模型描述
    std::string model_description;
    // 模型路径
    std::string path; 
    // 模型md5
    std::string md5; 
    // 模型参数
    FIBO_Audio param;
} FIBO_Audio_Model;

}  // namespace aisdk

#endif  // FIBO_MODEL_AUDIO_H_