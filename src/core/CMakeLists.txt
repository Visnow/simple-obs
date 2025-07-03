# 核心模块源文件
set(CORE_SOURCES
    Engine.cpp
    SceneImpl.cpp
    Logger.cpp
    VideoFrame.cpp
    AudioFrame.cpp
)

# 创建核心库
add_library(SimpleOBSCore STATIC ${CORE_SOURCES})

# 设置包含目录
target_include_directories(SimpleOBSCore PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src/core
)

# 链接依赖
target_link_libraries(SimpleOBSCore
    spdlog::spdlog
    ${PLATFORM_LIBS}
    OpenGL::GL
    Threads::Threads
) 