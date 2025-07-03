#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>

namespace SimpleOBS {

class Logger {
public:
    static Logger& getInstance();
    
    // 初始化日志系统
    bool initialize(const std::string& logDir = "logs", 
                   const std::string& logFile = "simpleobs.log",
                   spdlog::level::level_enum level = spdlog::level::info);
    
    // 关闭日志系统
    void shutdown();
    
    // 获取日志记录器
    std::shared_ptr<spdlog::logger> getLogger() const { return logger_; }
    
    // 便捷的日志方法
    template<typename... Args>
    void trace(const char* fmt, const Args&... args) {
        if (logger_) logger_->trace(fmt, args...);
    }
    
    template<typename... Args>
    void debug(const char* fmt, const Args&... args) {
        if (logger_) logger_->debug(fmt, args...);
    }
    
    template<typename... Args>
    void info(const char* fmt, const Args&... args) {
        if (logger_) logger_->info(fmt, args...);
    }
    
    template<typename... Args>
    void warn(const char* fmt, const Args&... args) {
        if (logger_) logger_->warn(fmt, args...);
    }
    
    template<typename... Args>
    void error(const char* fmt, const Args&... args) {
        if (logger_) logger_->error(fmt, args...);
    }
    
    template<typename... Args>
    void critical(const char* fmt, const Args&... args) {
        if (logger_) logger_->critical(fmt, args...);
    }
    
    // 设置日志级别
    void setLevel(spdlog::level::level_enum level);
    
    // 刷新日志
    void flush();

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::shared_ptr<spdlog::logger> logger_;
    bool initialized_ = false;
};

// 便捷宏定义 - 包含文件名、行号和函数名
#define LOG_TRACE(...)    SimpleOBS::Logger::getInstance().trace(__VA_ARGS__)
#define LOG_DEBUG(...)    SimpleOBS::Logger::getInstance().debug(__VA_ARGS__)
#define LOG_INFO(...)     SimpleOBS::Logger::getInstance().info(__VA_ARGS__)
#define LOG_WARN(...)     SimpleOBS::Logger::getInstance().warn(__VA_ARGS__)
#define LOG_ERROR(...)    SimpleOBS::Logger::getInstance().error(__VA_ARGS__)
#define LOG_CRITICAL(...) SimpleOBS::Logger::getInstance().critical(__VA_ARGS__)

// 详细日志宏 - 包含文件名、行号和函数名
#define LOG_TRACE_DETAIL(...)    SimpleOBS::Logger::getInstance().trace("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_DEBUG_DETAIL(...)    SimpleOBS::Logger::getInstance().debug("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO_DETAIL(...)     SimpleOBS::Logger::getInstance().info("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN_DETAIL(...)     SimpleOBS::Logger::getInstance().warn("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR_DETAIL(...)    SimpleOBS::Logger::getInstance().error("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_CRITICAL_DETAIL(...) SimpleOBS::Logger::getInstance().critical("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)

} // namespace SimpleOBS 