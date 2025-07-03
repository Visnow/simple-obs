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

// 基础类型定义
using FrameTime = std::chrono::microseconds;
using SourcePtr = std::shared_ptr<Source>;
using EncoderPtr = std::shared_ptr<Encoder>;
using OutputPtr = std::shared_ptr<Output>;
using FilterPtr = std::shared_ptr<Filter>;
using ScenePtr = std::shared_ptr<Scene>;

// 视频帧结构
struct VideoFrame {
    uint8_t* data[4];
    int linesize[4];
    int width;
    int height;
    FrameTime timestamp;
    int format;
};

// 音频帧结构
struct AudioFrame {
    float* data[8];
    int samples;
    int sample_rate;
    int channels;
    FrameTime timestamp;
};

// 基础接口类
class IBase {
public:
    virtual ~IBase() = default;
    virtual std::string getName() const = 0;
    virtual std::string getId() const = 0;
    virtual bool initialize() = 0;
    virtual void shutdown() = 0;
};

// 源接口
class Source : public IBase {
public:
    virtual ~Source() = default;
    virtual bool getVideoFrame(VideoFrame& frame) = 0;
    virtual bool getAudioFrame(AudioFrame& frame) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isActive() const = 0;
};

// 编码器接口
class Encoder : public IBase {
public:
    virtual ~Encoder() = default;
    virtual bool encodeFrame(const VideoFrame& frame) = 0;
    virtual bool encodeFrame(const AudioFrame& frame) = 0;
};

// 输出接口
class Output : public IBase {
public:
    virtual ~Output() = default;
    virtual bool start() = 0;
    virtual void stop() = 0;
    virtual bool isActive() const = 0;
};

// 滤镜接口
class Filter : public IBase {
public:
    virtual ~Filter() = default;
    virtual bool processVideoFrame(VideoFrame& frame) = 0;
    virtual bool processAudioFrame(AudioFrame& frame) = 0;
};

// 场景接口 - 纯虚类
class Scene : public IBase {
public:
    virtual ~Scene() = default;
    
    virtual void addSource(SourcePtr source) = 0;
    virtual void removeSource(SourcePtr source) = 0;
    virtual bool render(VideoFrame& frame) = 0;
    virtual bool render(AudioFrame& frame) = 0;
};

// 主引擎类
class Engine {
public:
    static Engine& getInstance();
    
    bool initialize();
    void shutdown();
    
    ScenePtr createScene(const std::string& name);
    SourcePtr createSource(const std::string& id, const std::string& name);
    EncoderPtr createEncoder(const std::string& id, const std::string& name);
    OutputPtr createOutput(const std::string& id, const std::string& name);
    FilterPtr createFilter(const std::string& id, const std::string& name);
    
    bool startStreaming();
    void stopStreaming();
    bool isStreaming() const;

private:
    Engine();
    ~Engine();
    Engine(const Engine&) = delete;
    Engine& operator=(const Engine&) = delete;
    
    class Impl;
    std::unique_ptr<Impl> pImpl;
};

} // namespace SimpleOBS 