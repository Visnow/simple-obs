/**
 * @file main.cpp
 * @brief SimpleOBS主程序入口
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 * 
 * @description
 * 本文件是SimpleOBS项目的主程序入口点。
 * 负责初始化系统、创建和管理组件、处理用户交互，以及优雅地关闭程序。
 * 
 * @note
 * - 使用RAII原则管理资源生命周期
 * - 提供异常安全的程序启动和关闭
 * - 支持命令行参数配置
 * - 集成完整的日志系统
 */

#include "SimpleOBS.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace SimpleOBS;

/**
 * @brief 程序启动和初始化
 * @return true表示启动成功，false表示启动失败
 * 
 * @details
 * 1. 初始化日志系统
 * 2. 初始化SimpleOBS引擎
 * 3. 创建默认场景和组件
 * 4. 验证系统状态
 * 
 * @note 使用RAII原则确保资源正确初始化
 */
bool initializeApplication() {
    LOG_INFO_DETAIL("Starting SimpleOBS application initialization");
    
    // 初始化日志系统
    if (!Logger::getInstance().initialize()) {
        std::cerr << "Failed to initialize logger system" << std::endl;
        return false;
    }
    
    LOG_INFO("SimpleOBS application starting...");
    LOG_INFO("Version: 1.0.0");
    LOG_INFO("Build date: {}", __DATE__);
    LOG_INFO("Build time: {}", __TIME__);
    
    // 初始化SimpleOBS引擎
    auto& engine = Engine::getInstance();
    if (!engine.initialize()) {
        LOG_ERROR("Failed to initialize SimpleOBS engine");
        return false;
    }
    
    LOG_INFO("SimpleOBS engine initialized successfully");
    
    // 创建默认场景
    auto scene = engine.createScene("Default Scene");
    if (!scene) {
        LOG_ERROR("Failed to create default scene");
        return false;
    }
    
    LOG_INFO("Default scene created successfully");
    
    return true;
}

/**
 * @brief 程序关闭和清理
 * @details 按正确顺序关闭所有组件，释放所有资源
 * 
 * @note 确保所有资源都被正确释放，避免内存泄漏
 */
void shutdownApplication() {
    LOG_INFO_DETAIL("Starting SimpleOBS application shutdown");
    
    // 停止流媒体（如果正在运行）
    auto& engine = Engine::getInstance();
    if (engine.isStreaming()) {
        LOG_INFO("Stopping streaming...");
        engine.stopStreaming();
    }
    
    // 关闭SimpleOBS引擎
    LOG_INFO("Shutting down SimpleOBS engine...");
    engine.shutdown();
    
    // 关闭日志系统（最后关闭）
    LOG_INFO("Shutting down logger system...");
    Logger::getInstance().shutdown();
    
    LOG_INFO("SimpleOBS application shutdown completed");
}

/**
 * @brief 演示场景操作
 * @details 展示如何创建和管理场景、源等组件
 * 
 * @note 这是一个演示函数，展示API的使用方法
 */
void demonstrateSceneOperations() {
    LOG_INFO_DETAIL("Demonstrating scene operations");
    
    auto& engine = Engine::getInstance();
    
    // 创建多个场景
    auto scene1 = engine.createScene("Scene 1");
    auto scene2 = engine.createScene("Scene 2");
    
    if (scene1 && scene2) {
        LOG_INFO("Created two scenes: '{}' and '{}'", scene1->getName(), scene2->getName());
        
        // 演示场景初始化
        if (scene1->initialize()) {
            LOG_INFO("Scene '{}' initialized successfully", scene1->getName());
        }
        
        if (scene2->initialize()) {
            LOG_INFO("Scene '{}' initialized successfully", scene2->getName());
        }
    }
    
    // 演示流媒体控制
    LOG_INFO("Starting streaming demonstration...");
    if (engine.startStreaming()) {
        LOG_INFO("Streaming started successfully");
        
        // 模拟流媒体运行一段时间
        std::this_thread::sleep_for(std::chrono::seconds(2));
        
        LOG_INFO("Stopping streaming...");
        engine.stopStreaming();
        LOG_INFO("Streaming stopped");
    } else {
        LOG_ERROR("Failed to start streaming");
    }
}

/**
 * @brief 主程序入口点
 * @param[in] argc 命令行参数数量
 * @param[in] argv 命令行参数数组
 * @return 程序退出码，0表示成功，非0表示错误
 * 
 * @details
 * 1. 解析命令行参数
 * 2. 初始化应用程序
 * 3. 执行主程序逻辑
 * 4. 处理异常和错误
 * 5. 优雅地关闭程序
 * 
 * @note 使用异常安全的设计，确保程序能够正确关闭
 */
int main(int argc, char* argv[]) {
    try {
        // 解析命令行参数（简单实现）
        LOG_INFO_DETAIL("SimpleOBS starting with {} command line arguments", argc);
        for (int i = 0; i < argc; ++i) {
            LOG_DEBUG("Argument {}: {}", i, argv[i]);
        }
        
        // 初始化应用程序
        if (!initializeApplication()) {
            std::cerr << "Failed to initialize SimpleOBS application" << std::endl;
            return 1;
        }
        
        LOG_INFO("SimpleOBS application initialized successfully");
        
        // 执行演示操作
        demonstrateSceneOperations();
        
        // 模拟程序运行
        LOG_INFO("SimpleOBS running... Press Ctrl+C to exit");
        
        // 在实际应用中，这里应该进入主事件循环
        // 为了演示，我们只是等待一段时间
        std::this_thread::sleep_for(std::chrono::seconds(3));
        
        LOG_INFO("SimpleOBS demonstration completed");
        
        // 关闭应用程序
        shutdownApplication();
        
        LOG_INFO("SimpleOBS application exited successfully");
        return 0;
        
    } catch (const std::exception& e) {
        // 捕获并记录所有异常
        std::cerr << "Fatal error: " << e.what() << std::endl;
        
        // 尝试记录错误日志（如果日志系统可用）
        try {
            LOG_CRITICAL("Fatal error occurred: {}", e.what());
            Logger::getInstance().flush();
        } catch (...) {
            // 如果日志系统也失败了，至少确保错误信息被输出
            std::cerr << "Failed to log error message" << std::endl;
        }
        
        return 1;
        
    } catch (...) {
        // 捕获所有其他异常
        std::cerr << "Unknown fatal error occurred" << std::endl;
        
        try {
            LOG_CRITICAL("Unknown fatal error occurred");
            Logger::getInstance().flush();
        } catch (...) {
            std::cerr << "Failed to log unknown error" << std::endl;
        }
        
        return 1;
    }
} 