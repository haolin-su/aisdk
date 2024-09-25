#ifndef _LOG_H_
#define _LOG_H_

#include <iostream>
#include <string>
#include <memory>  // for std::shared_ptr
#include "fmt/fmt.h" // 确保包含了fmt库的头文件

namespace aisdk {
    
class LogImpl;

class Log {
public:
    Log();
    ~Log();

    template <typename... Args>
    void log(int level,const char* format, Args&&... args){
        auto message = fmt::format(format, std::forward<Args>(args)...);
        PrivateLog(level,message);
    };
    static std::shared_ptr<Log> GetLog() {
        if (s_logger == nullptr) {
            s_logger = std::make_shared<Log>();
        }
        return s_logger;
    }

    // 设置日志级别
    void SetLevel(int level);
    int GetLevel();

private:
    std::unique_ptr<LogImpl> pImpl;

    static std::shared_ptr<Log> s_logger; 

    // 私有方法，用于格式化并记录日志
    void PrivateLog(int level,const std::string& message);
};

} // namespace aisdk


#define AISDK_LEVEL_TRACE 0
#define AISDK_LEVEL_DEBUG 1
#define AISDK_LEVEL_INFO 2
#define AISDK_LEVEL_WARN 3
#define AISDK_LEVEL_ERROR 4
#define AISDK_LEVEL_CRITICAL 5
#define AISDK_LEVEL_OFF 6

#define AISDK_LOG(level, ...) aisdk::Log::GetLog()->log(level, __VA_ARGS__)
#if !defined(AISDK_ACTIVE_LEVEL)
#define AISDK_ACTIVE_LEVEL AISDK_LEVEL_DEBUG
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_TRACE
#define AISDK_TRACE(...) AISDK_LOG(AISDK_LEVEL_TRACE, __VA_ARGS__)
#else
#define AISDK_TRACE(...) (void)0;
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_DEBUG
#define AISDK_DEBUG(...) AISDK_LOG(AISDK_LEVEL_DEBUG, __VA_ARGS__)
#else
#define AISDK_DEBUG(...) (void)0;
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_INFO
#define AISDK_INFO(...) AISDK_LOG(AISDK_LEVEL_INFO, __VA_ARGS__)
#else
#define AISDK_INFO(...) (void)0;
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_WARN
#define AISDK_WARN(...) AISDK_LOG(AISDK_LEVEL_WARN, __VA_ARGS__)
#else
#define AISDK_WARN(...) (void)0;
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_ERROR
#define AISDK_ERROR(...) AISDK_LOG(AISDK_LEVEL_ERROR, __VA_ARGS__)
#else
#define AISDK_ERROR(...) (void)0;
#endif

#if AISDK_ACTIVE_LEVEL <= AISDK_LEVEL_CRITICAL
#define AISDK_CRITICAL(...) AISDK_LOG(AISDK_LEVEL_CRITICAL, __VA_ARGS__)
#else
#define AISDK_CRITICAL(...) (void)0;
#endif

#define CHECK(a)                                                                                         \
    if (!(a))                                                                                            \
    {                                                                                                    \
        AISDK_ERROR("[{}] [{}] [{}]CHECK failed {} is nullptr", __FILE__, __FUNCTION__, __LINE__, a); \
    }

#define CHECK_NOTNULL(a)                                                                                 \
    if (NULL == (a))                                                                                     \
    {                                                                                                    \
        AISDK_ERROR("[{}] [{}] [{}]CHECK failed {} is nullptr", __FILE__, __FUNCTION__, __LINE__, a); \
    }

#define CHECK_NULL(a)                                                                                        \
    if (NULL != (a))                                                                                         \
    {                                                                                                        \
        AISDK_ERROR("[{}] [{}] [{}]CHECK failed {} is not nullptr", __FILE__, __FUNCTION__, __LINE__, a); \
    }

#define CHECK_EQ(a, b, comment, ...)          \
    if (!((a) == (b)))                        \
    {                                         \
        AISDK_ERROR(comment, __VA_ARGS__); \
    }

#define CHECK_NE(a, b, comment, ...)          \
    if (!((a) != (b)))                        \
    {                                         \
        AISDK_ERROR(comment, __VA_ARGS__); \
    }

#define CHECK_LT(a, b, comment, ...)          \
    if (!((a) < (b)))                         \
    {                                         \
        AISDK_ERROR(comment, __VA_ARGS__); \
    }

#define CHECK_GT(a, b, comment, ...)          \
    if (!((a) > (b)))                         \
    {                                         \
        AISDK_ERROR(comment, __VA_ARGS__); \
    }

// #define CHECK_LE(a, b, comment, ...)                        \
//    if(!((a) <= (b))) {                                      \
//         AISDK_ERROR(comment, __VA_ARGS__);                        \
//     }

#define CHECK_GE(a, b, comment, ...)          \
    if (!((a) >= (b)))                        \
    {                                         \
        AISDK_ERROR(comment, __VA_ARGS__); \
    }


#endif // _LOGGER_H_