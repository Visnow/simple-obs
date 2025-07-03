# 编码器模块源文件
set(ENCODERS_SOURCES
    BaseEncoder.cpp
    X264Encoder.cpp
)

# 创建编码器库
add_library(SimpleOBSEncoders STATIC ${ENCODERS_SOURCES})

# 设置包含目录
target_include_directories(SimpleOBSEncoders PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖
target_link_libraries(SimpleOBSEncoders
    SimpleOBSCore
) 