#include "SimpleOBS.h"
#include <iostream>

namespace SimpleOBS {

class BaseSource : public Source {
public:
    BaseSource(const std::string& name) : name_(name), active_(false) {}
    
    std::string getName() const override { return name_; }
    std::string getId() const override { return "base_source"; }
    
    bool initialize() override {
        std::cout << "Base source initializing: " << name_ << std::endl;
        return true;
    }
    
    void shutdown() override {
        stop();
        std::cout << "Base source shutting down: " << name_ << std::endl;
    }
    
    bool getVideoFrame(VideoFrame& frame) override {
        if (!active_) return false;
        
        // Basic implementation: generate solid color frame
        static uint8_t color_data[1920 * 1080 * 4]; // Assume 1920x1080 RGBA
        static bool initialized = false;
        
        if (!initialized) {
            // Generate red color
            for (int i = 0; i < 1920 * 1080; ++i) {
                color_data[i * 4 + 0] = 255; // R
                color_data[i * 4 + 1] = 0;   // G
                color_data[i * 4 + 2] = 0;   // B
                color_data[i * 4 + 3] = 255; // A
            }
            initialized = true;
        }
        
        frame.data[0] = color_data;
        frame.linesize[0] = 1920 * 4;
        frame.width = 1920;
        frame.height = 1080;
        frame.format = 0; // RGBA
        frame.timestamp = std::chrono::duration_cast<FrameTime>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        );
        
        return true;
    }
    
    bool getAudioFrame(AudioFrame& frame) override {
        if (!active_) return false;
        
        // Basic implementation: generate silence
        static float silence_data[48000]; // 1 second of silence data
        static bool initialized = false;
        
        if (!initialized) {
            for (int i = 0; i < 48000; ++i) {
                silence_data[i] = 0.0f;
            }
            initialized = true;
        }
        
        frame.data[0] = silence_data;
        frame.samples = 480;
        frame.sample_rate = 48000;
        frame.channels = 1;
        frame.timestamp = std::chrono::duration_cast<FrameTime>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
        );
        
        return true;
    }
    
    void start() override {
        active_ = true;
        std::cout << "Source started: " << name_ << std::endl;
    }
    
    void stop() override {
        active_ = false;
        std::cout << "Source stopped: " << name_ << std::endl;
    }
    
    bool isActive() const override {
        return active_;
    }

private:
    std::string name_;
    bool active_;
};

} // namespace SimpleOBS 