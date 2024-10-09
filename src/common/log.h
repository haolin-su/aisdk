#ifndef FIBO_LOG_H_
#define FIBO_LOG_H_

#include <iostream>
#include <memory>
#include <string>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>

namespace aisdk {

// struct log_execption : virtual std::exception {};

class SPDLOG {
private:
    SPDLOG() = default;
private:
    std::shared_ptr<spdlog::logger> logger_ptr_;
    void setLogLevel(const std::string& level);

public:
	static SPDLOG& getInstance() {
		static SPDLOG instance;
		return instance;
	}
    // 初始化一个默认日志文件logger: 日志路径；logger name; 日志等级；单个日志文件最大大小；回滚日志文件个数；日志是否线程安全；
	void init(std::string log_file_path,std::string logger_name, std::string level, size_t max_file_size, size_t max_files, bool mt_security = false); 
    std::shared_ptr<spdlog::logger> logger() { return logger_ptr_; }
}; // SPDLOG class

// #define AISDK_LOG_TRACE(...)       SPDLOG::getInstance().logger().get()->trace(__VA_ARGS__)
#define AISDK_LOG_TRACE1(...)       0
#define AISDK_LOG_TRACE(...)       SPDLOG::getInstance().logger().get()->info(__VA_ARGS__)
#define AISDK_LOG_DEBUG(...)       SPDLOG::getInstance().logger().get()->debug(__VA_ARGS__)
#define AISDK_LOG_INFO(...)        SPDLOG::getInstance().logger().get()->info(__VA_ARGS__)
#define AISDK_LOG_WARN(...)        SPDLOG::getInstance().logger().get()->warn(__VA_ARGS__)
#define AISDK_LOG_ERROR(...)       SPDLOG::getInstance().logger().get()->error(__VA_ARGS__)
#define AISDK_LOG_CRITICAL(...)    SPDLOG::getInstance().logger().get()->critical(__VA_ARGS__)
// #define AISDK_LOG_TRACE(...)       0
// #define AISDK_LOG_DEBUG(...)       0
// #define AISDK_LOG_INFO(...)        0
// #define AISDK_LOG_WARN(...)        0
// #define AISDK_LOG_ERROR(...)       0
// #define AISDK_LOG_CRITICAL(...)    0


} // Log namespace
#endif // FIBO_LOG_H_