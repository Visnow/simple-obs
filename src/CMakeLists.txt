# 添加子目录
add_subdirectory(core)
add_subdirectory(sources)
add_subdirectory(encoders)
add_subdirectory(outputs)
add_subdirectory(filters)

# 创建主程序
add_executable(SimpleOBS
    main.cpp
)

# 链接库
target_link_libraries(SimpleOBS
    SimpleOBSCore
    SimpleOBSSources
    SimpleOBSEncoders
    SimpleOBSOutputs
    SimpleOBSFilters
    ${PLATFORM_LIBS}
    OpenGL::GL
    Threads::Threads
) 