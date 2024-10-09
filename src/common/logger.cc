#include "log.h"
using namespace aisdk;

void aisdk::SPDLOG::init(std::string log_file_path, std::string logger_name, std::string level, size_t max_file_size, size_t max_files, bool mt_security)
{
    try {
        if (mt_security) {
            logger_ptr_ = spdlog::rotating_logger_mt(logger_name, log_file_path, max_file_size, max_files);
        }
        else {
            logger_ptr_ = spdlog::rotating_logger_st(logger_name, log_file_path, max_file_size, max_files);
        }
        setLogLevel(level);
        logger_ptr_->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s %!:%#] %v"); //设置格式:https://spdlog.docsforge.com/v1.x/3.custom-formatting/
    } 
    catch (const spdlog::spdlog_ex& ex) {
        std::cout << "Log initialization failed: " + std::string(ex.what()) << std::endl;
    }
}

void aisdk::SPDLOG::setLogLevel(const std::string &level)
{
    if (level == "trace") { // trace
        logger_ptr_->set_level(spdlog::level::trace);
        logger_ptr_->flush_on(spdlog::level::trace);
    }
    else if (level == "debug") { // debug
        logger_ptr_->set_level(spdlog::level::debug);
        logger_ptr_->flush_on(spdlog::level::debug);
    }
    else if (level == "info") { // info
        logger_ptr_->set_level(spdlog::level::info);
        logger_ptr_->flush_on(spdlog::level::info);
    }
    else if (level == "warn") { // warn
        logger_ptr_->set_level(spdlog::level::warn);
        logger_ptr_->flush_on(spdlog::level::warn);
    }
    else if (level == "error") { // error
        logger_ptr_->set_level(spdlog::level::err);
        logger_ptr_->flush_on(spdlog::level::err);
    }
    else if (level == "critical") { // critical
        logger_ptr_->set_level(spdlog::level::critical);
        logger_ptr_->flush_on(spdlog::level::critical);
    } else {
        // BOOST_THROW_EXCEPTION(log_execption()
        //     << err_str("level set error " + level)
        // );
        std::cout << "level set error " + level << std::endl;   
    }
}