#include "SceneImpl.h"
#include "Logger.h"
#include <algorithm>

namespace SimpleOBS {

SceneImpl::SceneImpl(const std::string& name) 
    : name_(name), initialized_(false) {
    LOG_DEBUG("SceneImpl constructed: {}", name_);
}

SceneImpl::~SceneImpl() {
    shutdown();
    LOG_DEBUG("SceneImpl destructed: {}", name_);
}

std::string SceneImpl::getName() const {
    return name_;
}

std::string SceneImpl::getId() const {
    return "scene_impl";
}

bool SceneImpl::initialize() {
    if (initialized_) {
        LOG_DEBUG("SceneImpl already initialized: {}", name_);
        return true;
    }
    
    LOG_INFO("SceneImpl initializing: {}", name_);
    initialized_ = true;
    return true;
}

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