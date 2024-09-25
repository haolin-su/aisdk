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
    ERROR_CODE_INIT_FAILED          = 1, // 初始化失败
    ERROR_CODE_INVALID_PARAM        = 2, // 参数错误
    ERROR_CODE_INVALID_NO_INIT      = 3, // 未初始化

    ERROR_CODE_INVALID_APP_ID       = 100, // 无效的应用ID
    ERROR_CODE_INVALID_APP_KEY      = 101, // 无效的应用Key
    ERROR_CODE_INVALID_APP_SECRET   = 102, // 无效的应用Secret
    ERROR_CODE_INVALID_LICENSE      = 103, // 无效的License

    ERROR_CODE_CV_INIT_FAILED       = 200, // CV初始化失败
    ERROR_CODE_CV_INVALID_PARAM     = 201, // CV参数错误
    ERROR_CODE_CV_INVALID_IMAGE     = 202, // CV无效的图像
    ERROR_CODE_CV_INVALID_MODEL     = 203, // CV无效的模型
    ERROR_CODE_CV_INVALID_RESULT    = 204, // CV无效的结果

    ERROR_CODE_NLP_INIT_FAILED      = 300, // NLP初始化失败
    ERROR_CODE_NLP_INVALID_PARAM    = 301, // NLP参数错误
    ERROR_CODE_NLP_INVALID_TEXT     = 302, // NLP无效的文本
    ERROR_CODE_NLP_INVALID_RESPONSE = 303, // NLP无效的响应

    ERROR_CODE_AUDIO_INIT_FAILED    = 400, // Audio初始化失败
    ERROR_CODE_AUDIO_INVALID_PARAM  = 401, // Audio参数错误
    ERROR_CODE_AUDIO_INVALID_AUDIO  = 402, // Audio无效的音频
    ERROR_CODE_AUDIO_INVALID_SPEECH = 403, // Audio无效的语音

    ERROR_CODE_RESOURCE_INIT_FAILED     = 500, // 资源初始化失败
    ERROR_CODE_RESOURCE_INVALID_PARAM   = 501, // 资源参数错误
    
    ERROR_CODE_OTHER_ERROR           = 1000, // 其他错误
};

typedef struct {
    int code; // 错误码
    std::string message; // 错误信息
} FIBO_Error;

std::string ErrorCodeToString(ErrorCode code);

}  // namespace aisdk

#endif  // FIBO_ERROR_H_