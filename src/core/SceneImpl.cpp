#include "SceneImpl.h"
#include <iostream>
#include <algorithm>

namespace SimpleOBS {

SceneImpl::SceneImpl(const std::string& name) 
    : name_(name), initialized_(false) {
    std::cout << "SceneImpl constructed: " << name_ << std::endl;
}

SceneImpl::~SceneImpl() {
    shutdown();
    std::cout << "SceneImpl destructed: " << name_ << std::endl;
}

std::string SceneImpl::getName() const {
    return name_;
}

std::string SceneImpl::getId() const {
    return "scene_impl";
}

bool SceneImpl::initialize() {
    if (initialized_) {
        std::cout << "SceneImpl already initialized: " << name_ << std::endl;
        return true;
    }
    
    std::cout << "SceneImpl initializing: " << name_ << std::endl;
    initialized_ = true;
    return true;
}

void SceneImpl::shutdown() {
    if (!initialized_) {
        return;
    }
    
    std::cout << "SceneImpl shutting down: " << name_ << std::endl;
    
    // Stop all sources
    for (auto& source : sources_) {
        if (source && source->isActive()) {
            source->stop();
        }
    }
    
    initialized_ = false;
}

void SceneImpl::addSource(SourcePtr source) {
    if (!source) {
        std::cout << "SceneImpl failed to add source: source is null" << std::endl;
        return;
    }
    
    // Check if already exists
    auto it = std::find(sources_.begin(), sources_.end(), source);
    if (it != sources_.end()) {
        std::cout << "SceneImpl source already exists: " << source->getName() << std::endl;
        return;
    }
    
    sources_.push_back(source);
    std::cout << "SceneImpl added source: " << source->getName() << " to scene: " << name_ << std::endl;
}

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
        std::cout << "SceneImpl removed source: " << source->getName() << " from scene: " << name_ << std::endl;
    }
}

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