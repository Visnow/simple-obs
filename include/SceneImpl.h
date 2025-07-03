/**
 * @file SceneImpl.h
 * @brief Scene接口的具体实现类
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 * 
 * @description
 * 本文件定义了Scene接口的具体实现类SceneImpl。
 * 负责管理场景中的多个源，实现视频和音频的合成渲染。
 * 
 * @note
 * - 继承自Scene接口，实现所有虚函数
 * - 支持多个源的添加、移除和管理
 * - 提供基础的视频和音频合成功能
 * - 线程安全，支持并发访问
 */

#pragma once

#include "SimpleOBS.h"
#include <vector>
#include <mutex>
#include <memory>

namespace SimpleOBS {

/**
 * @brief Scene接口的具体实现类
 * @details 管理场景中的多个源，实现视频和音频的合成渲染
 * 
 * @note 使用互斥锁保证线程安全
 * @note 支持动态添加和移除源
 * @note 提供基础的合成渲染功能
 */
class SceneImpl : public Scene {
public:
    /**
     * @brief 构造函数
     * @param[in] name 场景名称
     * @details 初始化场景，设置名称和内部状态
     */
    explicit SceneImpl(const std::string& name);
    
    /**
     * @brief 析构函数
     * @details 清理所有资源，确保正确释放
     */
    ~SceneImpl() override;
    
    /**
     * @brief 获取场景名称
     * @return 场景的人类可读名称
     */
    std::string getName() const override;
    
    /**
     * @brief 获取场景ID
     * @return 场景的唯一标识符
     */
    std::string getId() const override;
    
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
    bool initialize() override;
    
    /**
     * @brief 关闭场景
     * @details 停止所有源，释放所有资源
     */
    void shutdown() override;
    
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
    void addSource(SourcePtr source) override;
    
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
    void removeSource(SourcePtr source) override;
    
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
    bool render(VideoFrame& frame) override;
    
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
    bool render(AudioFrame& frame) override;
    
    /**
     * @brief 获取场景中的源数量
     * @return 当前场景中源的数量
     */
    size_t getSourceCount() const;
    
    /**
     * @brief 获取指定索引的源
     * @param[in] index 源在场景中的索引
     * @return 指定索引的源，如果索引无效则返回nullptr
     */
    SourcePtr getSource(size_t index) const;
    
    /**
     * @brief 根据名称查找源
     * @param[in] name 源名称
     * @return 找到的源，如果不存在则返回nullptr
     */
    SourcePtr findSource(const std::string& name) const;

private:
    std::string name_;                    ///< 场景名称
    std::string id_;                      ///< 场景唯一标识符
    bool initialized_;                    ///< 初始化状态标志
    
    mutable std::mutex sourcesMutex_;     ///< 保护源列表的互斥锁
    std::vector<SourcePtr> sources_;      ///< 场景中的源列表
    
    // 渲染相关成员
    VideoFrame renderBuffer_;             ///< 视频渲染缓冲区
    AudioFrame audioBuffer_;              ///< 音频渲染缓冲区
    bool renderBufferInitialized_;        ///< 渲染缓冲区初始化状态
    
    /**
     * @brief 初始化渲染缓冲区
     * @return true表示初始化成功，false表示初始化失败
     * 
     * @details
     * 1. 分配视频帧缓冲区内存
     * 2. 分配音频帧缓冲区内存
     * 3. 设置默认的帧参数
     * 4. 初始化缓冲区状态
     */
    bool initializeRenderBuffers();
    
    /**
     * @brief 清理渲染缓冲区
     * @details 释放所有缓冲区内存，重置状态
     */
    void cleanupRenderBuffers();
    
    /**
     * @brief 合成视频帧
     * @param[out] outputFrame 输出合成帧
     * @return true表示合成成功，false表示合成失败
     * 
     * @details
     * 1. 遍历所有源获取视频帧
     * 2. 按照Z-order进行合成
     * 3. 处理透明度混合
     * 4. 应用场景变换
     */
    bool compositeVideoFrames(VideoFrame& outputFrame);
    
    /**
     * @brief 合成音频帧
     * @param[out] outputFrame 输出合成音频帧
     * @return true表示合成成功，false表示合成失败
     * 
     * @details
     * 1. 遍历所有源获取音频帧
     * 2. 混合所有音频数据
     * 3. 应用音量控制
     * 4. 处理音频同步
     */
    bool compositeAudioFrames(AudioFrame& outputFrame);
};

} // namespace SimpleOBS 