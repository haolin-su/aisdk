#include "core/log.h"

#include <cstdlib>
#include <iostream>
#include <cstdarg> // for va_list, va_start, va_end
#include <memory>  // for std::shared_ptr
#include <filesystem>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/fmt.h"
#include "logger.h"

namespace aisdk{

std::shared_ptr<Log> Log::s_logger = nullptr; 

static void LoadEnvLevels() {
    auto p = std::getenv("AISDK_LOG_LEVEL");
    if (p) {
        const std::string str(p);
        if (str == "trace") {
            spdlog::set_level(spdlog::level::trace);
        } else if (str == "debug") {
            spdlog::set_level(spdlog::level::debug);
        } else if (str == "info") {
            spdlog::set_level(spdlog::level::info);
        } else if (str == "warn") {
            spdlog::set_level(spdlog::level::warn);
        } else if (str == "err") {
            spdlog::set_level(spdlog::level::err);
        } else if (str == "critical") {
            spdlog::set_level(spdlog::level::critical);
        } else if (str == "off") {
            spdlog::set_level(spdlog::level::off);
        }
    } else {
        spdlog::set_level(spdlog::level::err);
    }
    // spdlog::set_level(spdlog::level::err);
}

std::shared_ptr<spdlog::logger> CreateDefaultLogger() 
{
    LoadEnvLevels();
    constexpr const auto logger_name = "aisdk";
    std::shared_ptr<spdlog::logger> ptr;
    if (!spdlog::get(logger_name)) {
        // 日志器不存在，创建一个新的
        ptr = spdlog::stdout_color_mt(logger_name);
    }
    else {
        // 日志器已存在，直接获取
        ptr = spdlog::get(logger_name);
    }
    return ptr;
}

class LogImpl {
private:
    std::shared_ptr<spdlog::logger> ptr_;
public:
    LogImpl(){
        ptr_ = CreateDefaultLogger(); 
    };
    ~LogImpl(){
        ptr_->flush();
    };
    void log(int level,const std::string& msg) {
        ptr_->log(static_cast<spdlog::level::level_enum>(level) , msg);
    }
    void SetLevel(int level) {
        ptr_->set_level(static_cast<spdlog::level::level_enum>(level));
    }
    int GetLevel() {
        return ptr_->level();
    }
};

Log::Log() : pImpl(std::unique_ptr<LogImpl>(new LogImpl())) {}
Log::~Log() = default;

void Log::SetLevel(int level) {
    pImpl->SetLevel(level);
}

int Log::GetLevel() {
    return pImpl->GetLevel();
}

void Log::PrivateLog(int level,const std::string& message) {
    pImpl->log(level, message);
}

int SetLog(LogConfig log)
{
    Log::GetLog()->SetLevel(log.level);

    // TODO: 设置保存文件和参数

    return 0;
}

int GetLog(LogConfig &log)
{
    // log = Log::GetLog()->GetLog();
    return 0;
}

}