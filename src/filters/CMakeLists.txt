# 滤镜模块源文件
set(FILTERS_SOURCES
    BaseFilter.cpp
    CropFilter.cpp
    ScaleFilter.cpp
)

# 创建滤镜库
add_library(SimpleOBSFilters STATIC ${FILTERS_SOURCES})

# 设置包含目录
target_include_directories(SimpleOBSFilters PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# 链接依赖
target_link_libraries(SimpleOBSFilters
    SimpleOBSCore
) 