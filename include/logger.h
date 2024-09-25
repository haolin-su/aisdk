/******************************************************************************
  Copyright (C), 2024, Shenzhen G&T Industrial Development Co., Ltd

  File:      logger.h

  Author:  leo.li         
  Version: 1.0        
  Date:  2024.09
  
  Description:   logger APIs 

** History:
**Author (core ID)                Date          Number     Description of Changes
**-----------------------------------------------------------------------------
** leo.li                      09-Sep-2024     LIBtt54238   Init 
** -----------------------------------------------------------------------------
******************************************************************************/

#ifndef FIBO_LOGGER_H_
#define FIBO_LOGGER_H_

#include <iostream>
#include <string>

namespace aisdk {

enum LogLevel {
    LOG_LEVEL_TRACE = 0, // 调试
    LOG_LEVEL_DEBUG = 1, // 信息
    LOG_LEVEL_INFO = 2, // 警告#
    LOG_LEVEL_WARNING = 3, // 错误
    LOG_LEVEL_ERROR = 4, // 严重错误
    LOG_LEVEL_CRITICAL = 5, // 严重错误
};

typedef struct {
    int level; // 日志级别，参考LogLevel
    std::string file_path; // 文件路径
    int max_size; // 日志文件最大大小， 单位：MB，取值范围[0, 100MB]
    int max_count; // 日志文件最大数量， 单位：个，取值范围[1, 10]
    bool enable_console; // 是否输出到控制台
    bool enable_file; // 是否输出到文件
} LogConfig;

// 日志函数
// 功能说明：设置日志
// 参数说明：
// log：日志结构体，必须传入，非必须每个字段都设置
// 返回值：0表示成功，其他表示失败
int SetLog(LogConfig log);

// 日志函数
// 功能说明：获取日志
// 参数说明：
// log：日志结构体，必须传入，非必须每个字段都设置
// 返回值：0表示成功，其他表示失败
int GetLog(LogConfig &log);

} // namespace aisdk
#endif  // FIBO_LOGGER_H_
