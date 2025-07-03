# 源模块源文件
set(SOURCES_SOURCES
    BaseSource.cpp
    ColorSource.cpp
    ImageSource.cpp
)

# 创建源库
add_library(SimpleOBSSources STATIC ${SOURCES_SOURCES})

# 设置包含目录
target_include_directories(SimpleOBSSources PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖
target_link_libraries(SimpleOBSSources
    SimpleOBSCore
) 