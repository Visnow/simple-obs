/**
 * @file SimpleOBS.h
 * @brief SimpleOBS核心接口定义
 * @author SimpleOBS Team
 * @date 2025-07-04
 * @version 1.0.0
 * 
 * @description
 * 本文件定义了SimpleOBS项目的核心接口和数据结构。
 * 包含视频/音频帧结构、各种组件接口（源、编码器、输出、滤镜、场景）以及主引擎类。
 * 
 * @note
 * - 所有接口都继承自IBase基类，提供统一的命名和生命周期管理
 * - VideoFrame和AudioFrame使用struct定义，作为纯数据容器
 * - 使用智能指针管理对象生命周期，避免内存泄漏
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <functional>
#include <chrono>

namespace SimpleOBS {

// 前向声明
class Source;
class Encoder;
class Output;
class Filter;
class Scene;
struct VideoFrame;
struct AudioFrame;

/**
 * @brief 帧时间戳类型定义
 * @details 使用微秒精度的时间戳，适用于高精度音视频同步
 */
using FrameTime = std::chrono::microseconds;

/**
 * @brief 智能指针类型别名
 * @details 统一使用shared_ptr管理组件生命周期
 */
using SourcePtr = std::shared_ptr<Source>;
using EncoderPtr = std::shared_ptr<Encoder>;
using OutputPtr = std::shared_ptr<Output>;
using FilterPtr = std::shared_ptr<Filter>;
using ScenePtr = std::shared_ptr<Scene>;

/**
 * @brief 视频帧数据结构
 * @details 存储视频帧的像素数据和元信息，支持多种像素格式
 */
struct VideoFrame {
    uint8_t* data[4];      ///< 像素数据指针数组，支持YUV/RGB等格式
    int linesize[4];       ///< 每行字节数，用于处理对齐和步长
    int width;             ///< 帧宽度（像素）
    int height;            ///< 帧高度（像素）
    FrameTime timestamp;   ///< 时间戳，用于同步
    int format;            ///< 像素格式标识符
};

/**
 * @brief 音频帧数据结构
 * @details 存储音频帧的采样数据和元信息，支持多声道
 */
struct AudioFrame {
    float* data[8];        ///< 音频数据指针数组，支持多声道
    int samples;           ///< 采样点数
    int sample_rate;       ///< 采样率（Hz）
    int channels;          ///< 声道数
    FrameTime timestamp;   ///< 时间戳，用于同步
};

/**
 * @brief 基础接口类
 * @details 所有SimpleOBS组件的基类，提供统一的命名和生命周期管理接口
 */
class IBase {
public:
    virtual ~IBase() = default;
    
    /**
     * @brief 获取组件名称
     * @return 组件的人类可读名称
     */
    virtual std::string getName() const = 0;
    
    /**
     * @brief 获取组件ID
     * @return 组件的唯一标识符
     */
    virtual std::string getId() const = 0;
    
    /**
     * @brief 初始化组件
     * @return true表示初始化成功，false表示失败
     */
    virtual bool initialize() = 0;
    
    /**
     * @brief 关闭组件
     * @details 释放资源，停止所有活动
     */
    virtual void shutdown() = 0;
};

/**
 * @brief 源接口类
 * @details 负责产生视频和音频数据的组件，如摄像头、麦克风、文件等
 */
class Source : public IBase {
public:
    virtual ~Source() = default;
    
    /**
     * @brief 获取视频帧
     * @param[out] frame 输出视频帧数据
     * @return true表示成功获取帧，false表示无帧或错误
     */
    virtual bool getVideoFrame(VideoFrame& frame) = 0;
    
    /**
     * @brief 获取音频帧
     * @param[out] frame 输出音频帧数据
     * @return true表示成功获取帧，false表示无帧或错误
     */
    virtual bool getAudioFrame(AudioFrame& frame) = 0;
    
    /**
     * @brief 启动源
     * @details 开始产生数据流
     */
    virtual void start() = 0;
    
    /**
     * @brief 停止源
     * @details 停止产生数据流
     */
    virtual void stop() = 0;
    
    /**
     * @brief 检查源是否处于活动状态
     * @return true表示正在产生数据，false表示已停止
     */
    virtual bool isActive() const = 0;
};

/**
 * @brief 编码器接口类
 * @details 负责将原始音视频数据编码为特定格式，如H.264、AAC等
 */
class Encoder : public IBase {
public:
    virtual ~Encoder() = default;
    
    /**
     * @brief 编码视频帧
     * @param[in] frame 输入视频帧
     * @return true表示编码成功，false表示编码失败
     */
    virtual bool encodeFrame(const VideoFrame& frame) = 0;
    
    /**
     * @brief 编码音频帧
     * @param[in] frame 输入音频帧
     * @return true表示编码成功，false表示编码失败
     */
    virtual bool encodeFrame(const AudioFrame& frame) = 0;
};

/**
 * @brief 输出接口类
 * @details 负责将编码后的数据输出到目标，如文件、网络流等
 */
class Output : public IBase {
public:
    virtual ~Output() = default;
    
    /**
     * @brief 启动输出
     * @return true表示启动成功，false表示启动失败
     */
    virtual bool start() = 0;
    
    /**
     * @brief 停止输出
     * @details 关闭输出流，释放资源
     */
    virtual void stop() = 0;
    
    /**
     * @brief 检查输出是否处于活动状态
     * @return true表示正在输出，false表示已停止
     */
    virtual bool isActive() const = 0;
};

/**
 * @brief 滤镜接口类
 * @details 负责对音视频数据进行实时处理，如裁剪、缩放、滤镜效果等
 */
class Filter : public IBase {
public:
    virtual ~Filter() = default;
    
    /**
     * @brief 处理视频帧
     * @param[in,out] frame 输入输出视频帧，滤镜会直接修改此帧
     * @return true表示处理成功，false表示处理失败
     */
    virtual bool processVideoFrame(VideoFrame& frame) = 0;
    
    /**
     * @brief 处理音频帧
     * @param[in,out] frame 输入输出音频帧，滤镜会直接修改此帧
     * @return true表示处理成功，false表示处理失败
     */
    virtual bool processAudioFrame(AudioFrame& frame) = 0;
};

/**
 * @brief 场景接口类
 * @details 管理多个源的组合，负责渲染和合成最终的输出画面
 */
class Scene : public IBase {
public:
    virtual ~Scene() = default;
    
    /**
     * @brief 添加源到场景
     * @param[in] source 要添加的源
     */
    virtual void addSource(SourcePtr source) = 0;
    
    /**
     * @brief 从场景移除源
     * @param[in] source 要移除的源
     */
    virtual void removeSource(SourcePtr source) = 0;
    
    /**
     * @brief 渲染视频帧
     * @param[out] frame 输出的合成视频帧
     * @return true表示渲染成功，false表示渲染失败
     */
    virtual bool render(VideoFrame& frame) = 0;
    
    /**
     * @brief 渲染音频帧
     * @param[out] frame 输出的合成音频帧
     * @return true表示渲染成功，false表示渲染失败
     */
    virtual bool render(AudioFrame& frame) = 0;
};

/**
 * @brief 主引擎类
 * @details SimpleOBS的核心引擎，负责管理所有组件和协调整个系统
 * 
 * @note 使用单例模式，确保全局只有一个引擎实例
 * @note 使用PIMPL模式隐藏实现细节，提高编译速度和接口稳定性
 */
class Engine {
public:
    /**
     * @brief 获取引擎单例实例
     * @return 引擎的全局唯一实例
     */
    static Engine& getInstance();
    
    /**
     * @brief 初始化引擎
     * @return true表示初始化成功，false表示初始化失败
     */
    bool initialize();
    
    /**
     * @brief 关闭引擎
     * @details 关闭所有组件，释放所有资源
     */
    void shutdown();
    
    /**
     * @brief 创建场景
     * @param[in] name 场景名称
     * @return 场景的智能指针，失败时返回nullptr
     */
    ScenePtr createScene(const std::string& name);
    
    /**
     * @brief 创建源
     * @param[in] id 源类型ID，如"color_source"、"image_source"等
     * @param[in] name 源名称
     * @return 源的智能指针，失败时返回nullptr
     */
    SourcePtr createSource(const std::string& id, const std::string& name);
    
    /**
     * @brief 创建编码器
     * @param[in] id 编码器类型ID，如"x264"、"aac"等
     * @param[in] name 编码器名称
     * @return 编码器的智能指针，失败时返回nullptr
     */
    EncoderPtr createEncoder(const std::string& id, const std::string& name);
    
    /**
     * @brief 创建输出
     * @param[in] id 输出类型ID，如"rtmp"、"file"等
     * @param[in] name 输出名称
     * @return 输出的智能指针，失败时返回nullptr
     */
    OutputPtr createOutput(const std::string& id, const std::string& name);
    
    /**
     * @brief 创建滤镜
     * @param[in] id 滤镜类型ID，如"crop"、"scale"等
     * @param[in] name 滤镜名称
     * @return 滤镜的智能指针，失败时返回nullptr
     */
    FilterPtr createFilter(const std::string& id, const std::string& name);
    
    /**
     * @brief 开始流媒体
     * @return true表示启动成功，false表示启动失败
     */
    bool startStreaming();
    
    /**
     * @brief 停止流媒体
     */
    void stopStreaming();
    
    /**
     * @brief 检查是否正在流媒体
     * @return true表示正在流媒体，false表示已停止
     */
    bool isStreaming() const;

private:
    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
    class Impl;
    std::unique_ptr<Impl> pImpl;  ///< PIMPL实现指针
};

} // namespace SimpleOBS 