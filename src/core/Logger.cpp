/**
 * @file Logger.cpp
 * @brief SimpleOBS日志系统实现
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 *
 * @description
 * 本文件实现了SimpleOBS的日志系统，基于spdlog库。
 * 提供控制台和文件双重输出，支持日志级别控制和文件轮转。
 *
 * @note
 * - 使用单例模式确保全局唯一实例
 * - 支持日志文件轮转，避免单个文件过大
 * - 提供详细的日志格式，包含时间戳和级别信息
 * - 线程安全，支持多线程并发日志输出
 */

#include "Logger.h"
#include <filesystem>
#include <iostream>

namespace SimpleOBS {

/**
 * @brief 获取日志管理器单例实例
 * @return 日志管理器的全局唯一实例
 *
 * @note 使用局部静态变量确保线程安全的单例实现
 */
Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

/**
 * @brief 初始化日志系统
 * @param[in] logDir 日志文件目录，默认为"logs"
 * @param[in] logFile 日志文件名，默认为"simpleobs.log"
 * @param[in] level 日志级别，默认为info级别
 * @return true表示初始化成功，false表示初始化失败
 *
 * @details
 * 1. 创建日志目录（如果不存在）
 * 2. 配置控制台输出sink
 * 3. 配置文件输出sink（支持轮转）
 * 4. 创建日志记录器并设置格式
 * 5. 设置日志级别
 *
 * @note 会自动创建日志目录
 * @note 日志文件大小超过5MB时会自动轮转，最多保留3个历史文件
 */
bool Logger::initialize(const std::string& logDir,
                       const std::string& logFile,
                       spdlog::level::level_enum level) {

    if (initialized_) {
        LOG_WARN("Logger already initialized");
        return true;
    }

    try {
        // 创建日志目录
        std::filesystem::create_directories(logDir);

        // 创建sink列表
        std::vector<spdlog::sink_ptr> sinks;

        // 控制台输出sink（彩色）
        auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        console_sink->set_level(level);
        console_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] %v");
        sinks.push_back(console_sink);

        // 文件输出sink（支持轮转）
        auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            logDir + "/" + logFile,
            5 * 1024 * 1024,  // 5MB
            3                  // 保留3个历史文件
        );
        file_sink->set_level(level);
        file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
        sinks.push_back(file_sink);

        // 创建日志记录器
        logger_ = std::make_shared<spdlog::logger>("SimpleOBS", sinks.begin(), sinks.end());
        logger_->set_level(level);

        // 设置为全局默认日志记录器
        spdlog::set_default_logger(logger_);

        initialized_ = true;

        LOG_INFO("Logger initialized successfully");
        LOG_INFO("Log directory: {}", logDir);
        LOG_INFO("Log file: {}", logFile);
        LOG_INFO("Log level: {}", spdlog::level::to_string_view(level));

        return true;

    } catch (const std::exception& e) {
        std::cerr << "Failed to initialize logger: " << e.what() << std::endl;
        return false;
    }
}

/**
 * @brief 关闭日志系统
 * @details 刷新所有日志，释放资源
 *
 * @note 确保所有缓存的日志都被写入输出目标
 */
void Logger::shutdown() {
    if (!initialized_) {
        return;
    }

    LOG_INFO("Shutting down logger");

    if (logger_) {
        logger_->flush();
        logger_.reset();
    }

    initialized_ = false;
}

/**
 * @brief 设置日志级别
 * @param[in] level 新的日志级别
 *
 * @details
 * 1. 验证日志记录器是否有效
 * 2. 设置新的日志级别
 * 3. 记录级别变更日志
 */
void Logger::setLevel(spdlog::level::level_enum level) {
    if (logger_) {
        logger_->set_level(level);
        LOG_INFO("Log level changed to: {}", spdlog::level::to_string_view(level));
    }
}

/**
 * @brief 刷新日志缓冲区
 * @details 强制将所有缓存的日志写入输出目标
 *
 * @note 在程序退出前调用此函数确保所有日志都被写入
 */
void Logger::flush() {
    if (logger_) {
        logger_->flush();
    }
}

} // namespace SimpleOBS
