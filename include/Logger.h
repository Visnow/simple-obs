/**
 * @file Logger.h
 * @brief SimpleOBS日志系统接口
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 * 
 * @description
 * 本文件定义了SimpleOBS项目的日志系统接口。
 * 基于spdlog库实现，支持控制台和文件双重输出，提供详细的日志格式和级别控制。
 * 
 * @note
 * - 使用单例模式确保全局唯一的日志实例
 * - 支持日志文件轮转，避免单个文件过大
 * - 提供便捷的日志宏，简化调用
 * - 支持文件名、行号、函数名的详细溯源
 */

#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <memory>
#include <string>

namespace SimpleOBS {

/**
 * @brief 日志管理器类
 * @details 基于spdlog的单例日志管理器，提供统一的日志接口
 * 
 * @note 线程安全，支持多线程并发日志输出
 * @note 支持日志级别动态调整
 * @note 自动管理日志文件轮转
 */
class Logger {
public:
    /**
     * @brief 获取日志管理器单例实例
     * @return 日志管理器的全局唯一实例
     */
    static Logger& getInstance();
    
    /**
     * @brief 初始化日志系统
     * @param[in] logDir 日志文件目录，默认为"logs"
     * @param[in] logFile 日志文件名，默认为"simpleobs.log"
     * @param[in] level 日志级别，默认为info级别
     * @return true表示初始化成功，false表示初始化失败
     * 
     * @note 会自动创建日志目录
     * @note 日志文件大小超过5MB时会自动轮转，最多保留3个历史文件
     */
    bool initialize(const std::string& logDir = "logs", 
                   const std::string& logFile = "simpleobs.log",
                   spdlog::level::level_enum level = spdlog::level::info);
    
    /**
     * @brief 关闭日志系统
     * @details 刷新所有日志，释放资源
     */
    void shutdown();
    
    /**
     * @brief 获取底层日志记录器
     * @return spdlog日志记录器的智能指针
     */
    std::shared_ptr<spdlog::logger> getLogger() const { return logger_; }
    
    /**
     * @brief 输出TRACE级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void trace(const char* fmt, const Args&... args) {
        if (logger_) logger_->trace(fmt, args...);
    }
    
    /**
     * @brief 输出DEBUG级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void debug(const char* fmt, const Args&... args) {
        if (logger_) logger_->debug(fmt, args...);
    }
    
    /**
     * @brief 输出INFO级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void info(const char* fmt, const Args&... args) {
        if (logger_) logger_->info(fmt, args...);
    }
    
    /**
     * @brief 输出WARN级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void warn(const char* fmt, const Args&... args) {
        if (logger_) logger_->warn(fmt, args...);
    }
    
    /**
     * @brief 输出ERROR级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void error(const char* fmt, const Args&... args) {
        if (logger_) logger_->error(fmt, args...);
    }
    
    /**
     * @brief 输出CRITICAL级别日志
     * @tparam Args 可变参数类型
     * @param[in] fmt 格式化字符串
     * @param[in] args 格式化参数
     */
    template<typename... Args>
    void critical(const char* fmt, const Args&... args) {
        if (logger_) logger_->critical(fmt, args...);
    }
    
    /**
     * @brief 设置日志级别
     * @param[in] level 新的日志级别
     */
    void setLevel(spdlog::level::level_enum level);
    
    /**
     * @brief 刷新日志缓冲区
     * @details 强制将所有缓存的日志写入输出目标
     */
    void flush();

private:
    Logger() = default;
    ~Logger() = default;
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    
    std::shared_ptr<spdlog::logger> logger_;  ///< 底层spdlog日志记录器
    bool initialized_ = false;                ///< 初始化状态标志
};

/**
 * @brief 便捷日志宏 - 基础版本
 * @details 提供简单的日志输出，不包含文件位置信息
 */
#define LOG_TRACE(...)    SimpleOBS::Logger::getInstance().trace(__VA_ARGS__)
#define LOG_DEBUG(...)    SimpleOBS::Logger::getInstance().debug(__VA_ARGS__)
#define LOG_INFO(...)     SimpleOBS::Logger::getInstance().info(__VA_ARGS__)
#define LOG_WARN(...)     SimpleOBS::Logger::getInstance().warn(__VA_ARGS__)
#define LOG_ERROR(...)    SimpleOBS::Logger::getInstance().error(__VA_ARGS__)
#define LOG_CRITICAL(...) SimpleOBS::Logger::getInstance().critical(__VA_ARGS__)

/**
 * @brief 详细日志宏 - 包含文件名、行号和函数名
 * @details 提供详细的日志输出，便于调试和问题定位
 * 
 * @note 使用__FILE__、__LINE__、__FUNCTION__宏获取位置信息
 * @note 格式：[文件名:行号:函数名] 日志内容
 */
#define LOG_TRACE_DETAIL(...)    SimpleOBS::Logger::getInstance().trace("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_DEBUG_DETAIL(...)    SimpleOBS::Logger::getInstance().debug("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_INFO_DETAIL(...)     SimpleOBS::Logger::getInstance().info("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN_DETAIL(...)     SimpleOBS::Logger::getInstance().warn("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR_DETAIL(...)    SimpleOBS::Logger::getInstance().error("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)
#define LOG_CRITICAL_DETAIL(...) SimpleOBS::Logger::getInstance().critical("[{}:{}:{}] " __VA_ARGS__, __FILE__, __LINE__, __FUNCTION__)

} // namespace SimpleOBS 