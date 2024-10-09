/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      error.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   error code 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_ERROR_H_
#define FIBO_ERROR_H_

#include <string>

namespace aisdk {

enum ErrorCode {
    ERROR_CODE_OK                   = 0, // 成功
    ERROR_CODE_INIT_FAILED          = 1, // 通用初始化失败
    ERROR_CODE_INVALID_PARAM        = 2, // 通用参数错误
    ERROR_CODE_INVALID_NO_INIT      = 3, // 通用未初始化

    ERROR_CODE_INVALID_APP_ID       = 100, // 无效的应用ID
    ERROR_CODE_INVALID_APP_KEY      = 101, // 无效的应用Key
    ERROR_CODE_INVALID_APP_SECRET   = 102, // 无效的应用Secret
    ERROR_CODE_INVALID_LICENSE      = 103, // 无效的License

    ERROR_CODE_CV_INIT_FAILED       = 200, // CV初始化失败
    ERROR_CODE_CV_INIT_ALREADY      = 201, // CV已经初始化
    ERROR_CODE_CV_NO_INIT           = 202, // CV未初始化
    ERROR_CODE_CV_INVALID_PARAM     = 203, // CV参数错误
    ERROR_CODE_CV_INVALID_MODEL     = 204, // CV无效的模型
    ERROR_CODE_CV_INVALID_IMAGE     = 205, // CV无效的图像
    ERROR_CODE_CV_INVALID_RESULT    = 206, // CV无效的结果
    ERROR_CODE_CV_WAIT_TIMEOUT      = 207, // CV等待超时
    
    ERROR_CODE_NLP_INIT_FAILED      = 300, // NLP初始化失败
    ERROR_CODE_NLP_INIT_ALREADY     = 301, // NLP已经初始化
    ERROR_CODE_NLP_NO_INIT          = 302, // NLP未初始化
    ERROR_CODE_NLP_INVALID_MODEL    = 303, // NLP无效的模型
    ERROR_CODE_NLP_INVALID_PARAM    = 304, // NLP参数错误
    ERROR_CODE_NLP_INVALID_TEXT     = 305, // NLP无效的文本
    ERROR_CODE_NLP_INVALID_RESPONSE = 306, // NLP无效的响应
    ERROR_CODE_NLP_LONG_TEXT        = 307, // NLP文本过长
    ERROR_CODE_NLP_WAIT_TIMEOUT     = 308, // NLP等待超时

    ERROR_CODE_AUDIO_INIT_FAILED    = 400, // Audio初始化失败
    ERROR_CODE_AUDIO_INIT_ALREADY   = 401, // Audio已经初始化
    ERROR_CODE_AUDIO_NO_INIT        = 402, // Audio未初始化
    ERROR_CODE_AUDIO_INVALID_MODEL  = 403, // Audio无效的模型
    ERROR_CODE_AUDIO_INVALID_PARAM  = 404, // Audio参数错误
    ERROR_CODE_AUDIO_INVALID_AUDIO  = 405, // Audio无效的音频
    ERROR_CODE_AUDIO_INVALID_SPEECH = 406, // Audio无效的语音
    ERROR_CODE_AUDIO_LONG_SPEECH    = 407, // Audio语音过长
    ERROR_CODE_AUDIO_WAIT_TIMEOUT   = 408, // Audio等待超时

    ERROR_CODE_RESOURCE_INIT_FAILED     = 500, // 资源初始化失败
    ERROR_CODE_RESOURCE_INVALID_PATH    = 501, // 资源释放失败
    ERROR_CODE_RESOURCE_INVALID_PARAM   = 502, // 资源参数错误
    ERROR_CODE_RESOURCE_NO_MEMORY       = 503, // 内存不足
    
    ERROR_CODE_OTHER_ERROR           = 1000, // 其他错误
};

typedef struct {
    int code; // 错误码
    std::string message; // 错误信息
} FIBO_Error;

std::string ErrorCodeToString(ErrorCode code);

}  // namespace aisdk

#endif  // FIBO_ERROR_H_