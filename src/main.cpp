#include "SimpleOBS.h"
#include "Logger.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    // Initialize logger
    auto& logger = SimpleOBS::Logger::getInstance();
    if (!logger.initialize()) {
        std::cerr << "Failed to initialize logger!" << std::endl;
        return -1;
    }
    
    LOG_INFO_DETAIL("=== SimpleOBS Starting ===");
    
    auto& engine = SimpleOBS::Engine::getInstance();
    
    // Initialize engine
    if (!engine.initialize()) {
        LOG_ERROR_DETAIL("Engine initialization failed!");
        return -1;
    }
    
    LOG_INFO_DETAIL("Engine initialized successfully!");
    
    // Create scene
    auto scene = engine.createScene("Main Scene");
    if (!scene) {
        LOG_ERROR_DETAIL("Scene creation failed!");
        return -1;
    }
    
    LOG_INFO_DETAIL("Scene created successfully!");
    
    // Create source
    auto source = engine.createSource("color_source", "Color Source");
    if (!source) {
        LOG_WARN_DETAIL("Source creation failed (expected, source types not implemented yet)");
    }
    
    // Create encoder
    auto encoder = engine.createEncoder("x264", "H.264 Encoder");
    if (!encoder) {
        LOG_WARN_DETAIL("Encoder creation failed (expected, encoder types not implemented yet)");
    }
    
    // Create output
    auto output = engine.createOutput("rtmp", "RTMP Output");
    if (!output) {
        LOG_WARN_DETAIL("Output creation failed (expected, output types not implemented yet)");
    }
    
    // Simulate running for a while
    LOG_INFO_DETAIL("Simulating run for 5 seconds...");
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Shutdown engine
    engine.shutdown();
    
    LOG_INFO_DETAIL("=== SimpleOBS Shutdown Complete ===");
    
    // Shutdown logger
    logger.shutdown();
    
    return 0;
} 