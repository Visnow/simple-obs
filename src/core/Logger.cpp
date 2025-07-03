#include "Logger.h"
#include <filesystem>
#include <iostream>

namespace SimpleOBS {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

bool Logger::initialize(const std::string& logDir, 
                       const std::string& logFile,
                       spdlog::level::level_enum level) {
    if (initialized_) {
        return true;
    }
    
    try {
        // 创建日志目录
        std::filesystem::create_directories(logDir);
        
        // 创建控制台输出
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(level);
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%t] [%s:%#] %v");
        
        // 创建文件输出（带轮转）
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logDir + "/" + logFile, 
            1024 * 1024 * 5,  // 5MB per file
            3                  // Keep 3 files
        );
        file_sink->set_level(level);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] [%s:%#] %v");
        
        // 创建多输出日志记录器
        std::vector<spdlog::sink_ptr> sinks {console_sink, file_sink};
        logger_ = std::make_shared<spdlog::logger>("SimpleOBS", sinks.begin(), sinks.end());
        logger_->set_level(level);
        
        // 设置为全局默认日志记录器
        spdlog::set_default_logger(logger_);
        
        initialized_ = true;
        
        LOG_INFO("Logger initialized successfully. Log directory: {}", logDir);
        LOG_INFO("Log level set to: {}", spdlog::level::to_string_view(level));
        
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
        return false;
    }
}

void Logger::shutdown() {
    if (logger_) {
        LOG_INFO("Logger shutting down...");
        logger_->flush();
        spdlog::shutdown();
        logger_.reset();
        initialized_ = false;
    }
}

void Logger::setLevel(spdlog::level::level_enum level) {
    if (logger_) {
        logger_->set_level(level);
        LOG_INFO("Log level changed to: {}", spdlog::level::to_string_view(level));
    }
}

void Logger::flush() {
    if (logger_) {
        logger_->flush();
    }
}

} // namespace SimpleOBS 