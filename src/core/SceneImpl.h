#pragma once

#include "SimpleOBS.h"
#include <memory>
#include <vector>
#include <string>

namespace SimpleOBS {

// Scene接口的具体实现类
class SceneImpl : public Scene {
public:
    explicit SceneImpl(const std::string& name);
    ~SceneImpl() override;

    // IBase接口实现
    std::string getName() const override;
    std::string getId() const override;
    bool initialize() override;
    void shutdown() override;

    // Scene接口实现
    void addSource(SourcePtr source) override;
    void removeSource(SourcePtr source) override;
    bool render(VideoFrame& frame) override;
    bool render(AudioFrame& frame) override;

private:
    std::string name_;
    std::vector<SourcePtr> sources_;
    bool initialized_;
};

} // namespace SimpleOBS
