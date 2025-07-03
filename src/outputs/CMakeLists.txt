# 输出模块源文件
set(OUTPUTS_SOURCES
    BaseOutput.cpp
    RTMPOutput.cpp
)

# 创建输出库
add_library(SimpleOBSOutputs STATIC ${OUTPUTS_SOURCES})

# 设置包含目录
target_include_directories(SimpleOBSOutputs PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖
target_link_libraries(SimpleOBSOutputs
    SimpleOBSCore
) 