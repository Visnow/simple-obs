#include "SimpleOBS.h"
#include <iostream>
#include <thread>
#include <chrono>

int main() {
    std::cout << "=== SimpleOBS Starting ===" << std::endl;
    
    auto& engine = SimpleOBS::Engine::getInstance();
    
    // Initialize engine
    if (!engine.initialize()) {
        std::cerr << "Engine initialization failed!" << std::endl;
        return -1;
    }
    
    std::cout << "Engine initialized successfully!" << std::endl;
    
    // Create scene
    auto scene = engine.createScene("Main Scene");
    if (!scene) {
        std::cerr << "Scene creation failed!" << std::endl;
        return -1;
    }
    
    std::cout << "Scene created successfully!" << std::endl;
    
    // Create source
    auto source = engine.createSource("color_source", "Color Source");
    if (!source) {
        std::cout << "Source creation failed (expected, source types not implemented yet)" << std::endl;
    }
    
    // Create encoder
    auto encoder = engine.createEncoder("x264", "H.264 Encoder");
    if (!encoder) {
        std::cout << "Encoder creation failed (expected, encoder types not implemented yet)" << std::endl;
    }
    
    // Create output
    auto output = engine.createOutput("rtmp", "RTMP Output");
    if (!output) {
        std::cout << "Output creation failed (expected, output types not implemented yet)" << std::endl;
    }
    
    // Simulate running for a while
    std::cout << "Simulating run for 5 seconds..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(5));
    
    // Shutdown engine
    engine.shutdown();
    
    std::cout << "=== SimpleOBS Shutdown Complete ===" << std::endl;
    return 0;
} 