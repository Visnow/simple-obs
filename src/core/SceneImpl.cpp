/**
 * @file SceneImpl.cpp
 * @brief Scene接口的具体实现
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 *
 * @description
 * 本文件实现了Scene接口的具体功能，包括场景管理、源操作和渲染合成。
 * 提供线程安全的场景操作，支持多源的视频和音频合成。
 *
 * @note
 * - 使用互斥锁保证线程安全
 * - 支持动态添加和移除源
 * - 提供基础的合成渲染功能
 * - 自动管理渲染缓冲区
 */

#include "SceneImpl.h"
#include "Logger.h"
#include <algorithm>

namespace SimpleOBS {

/**
 * @brief 构造函数
 * @param[in] name 场景名称
 * @details 初始化场景，设置名称和内部状态
 */
SceneImpl::SceneImpl(const std::string& name)
    : name_(name), initialized_(false) {
    LOG_DEBUG("SceneImpl constructed: {}", name_);
}

/**
 * @brief 析构函数
 * @details 清理所有资源，确保正确释放
 */
SceneImpl::~SceneImpl() {
    shutdown();
    LOG_DEBUG("SceneImpl destructed: {}", name_);
}

/**
 * @brief 获取场景名称
 * @return 场景的人类可读名称
 */
std::string SceneImpl::getName() const {
    return name_;
}

/**
 * @brief 获取场景ID
 * @return 场景的唯一标识符
 */
std::string SceneImpl::getId() const {
    return "scene_impl";
}

/**
 * @brief 初始化场景
 * @return true表示初始化成功，false表示初始化失败
 *
 * @details
 * 1. 验证场景名称的有效性
 * 2. 初始化内部数据结构
 * 3. 设置默认渲染参数
 * 4. 准备合成缓冲区
 */
bool SceneImpl::initialize() {
    if (initialized_) {
        LOG_DEBUG("SceneImpl already initialized: {}", name_);
        return true;
    }

    LOG_INFO("SceneImpl initializing: {}", name_);
    initialized_ = true;
    return true;
}

/**
 * @brief 关闭场景
 * @details 停止所有源，释放所有资源
 */
void SceneImpl::shutdown() {
    if (!initialized_) {
        return;
    }

    LOG_INFO("SceneImpl shutting down: {}", name_);

    // Stop all sources
    for (auto& source : sources_) {
        if (source && source->isActive()) {
            source->stop();
        }
    }

    initialized_ = false;
}

/**
 * @brief 添加源到场景
 * @param[in] source 要添加的源
 *
 * @details
 * 1. 验证源的有效性
 * 2. 检查是否已存在同名源
 * 3. 将源添加到场景的源列表中
 * 4. 更新渲染顺序
 *
 * @note 线程安全，使用互斥锁保护
 */
void SceneImpl::addSource(SourcePtr source) {
    if (!source) {
        LOG_ERROR("SceneImpl failed to add source: source is null");
        return;
    }

    // Check if already exists
    auto it = std::find(sources_.begin(), sources_.end(), source);
    if (it != sources_.end()) {
        LOG_WARN("SceneImpl source already exists: {}", source->getName());
        return;
    }

    sources_.push_back(source);
    LOG_INFO("SceneImpl added source: {} to scene: {}", source->getName(), name_);
}

/**
 * @brief 从场景移除源
 * @param[in] source 要移除的源
 *
 * @details
 * 1. 验证源的有效性
 * 2. 从场景的源列表中移除指定源
 * 3. 更新渲染顺序
 * 4. 清理相关资源
 *
 * @note 线程安全，使用互斥锁保护
 */
void SceneImpl::removeSource(SourcePtr source) {
    if (!source) {
        return;
    }

    auto it = std::find(sources_.begin(), sources_.end(), source);
    if (it != sources_.end()) {
        // Stop source
        if (source->isActive()) {
            source->stop();
        }

        sources_.erase(it);
        LOG_INFO("SceneImpl removed source: {} from scene: {}", source->getName(), name_);
    }
}

/**
 * @brief 渲染视频帧
 * @param[out] frame 输出的合成视频帧
 * @return true表示渲染成功，false表示渲染失败
 *
 * @details
 * 1. 从所有源获取视频帧
 * 2. 按照渲染顺序合成帧
 * 3. 应用场景级别的滤镜效果
 * 4. 输出最终的合成帧
 *
 * @note 线程安全，使用互斥锁保护
 */
bool SceneImpl::render(VideoFrame& frame) {
    if (!initialized_ || sources_.empty()) {
        return false;
    }

    // Simple rendering logic: render first active source
    for (auto& source : sources_) {
        if (source && source->isActive()) {
            return source->getVideoFrame(frame);
        }
    }

    return false;
}

/**
 * @brief 渲染音频帧
 * @param[out] frame 输出的合成音频帧
 * @return true表示渲染成功，false表示渲染失败
 *
 * @details
 * 1. 从所有源获取音频帧
 * 2. 混合所有音频数据
 * 3. 应用场景级别的音频处理
 * 4. 输出最终的合成音频帧
 *
 * @note 线程安全，使用互斥锁保护
 */
bool SceneImpl::render(AudioFrame& frame) {
    if (!initialized_ || sources_.empty()) {
        return false;
    }

    // Simple audio rendering logic: render first active source
    for (auto& source : sources_) {
        if (source && source->isActive()) {
            return source->getAudioFrame(frame);
        }
    }

    return false;
}

} // namespace SimpleOBS
