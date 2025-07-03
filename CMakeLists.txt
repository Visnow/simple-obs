cmake_minimum_required(VERSION 3.16)
project(SimpleOBS VERSION 1.0.0 LANGUAGES CXX)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 编译选项
if(MSVC)
    add_compile_options(/W4 /utf-8)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 包含目录
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/third_party)

# 查找依赖包
find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)

# 平台特定设置
if(WIN32)
    # Windows 特定设置
    add_definitions(-DWIN32_LEAN_AND_MEAN)
    add_definitions(-DNOMINMAX)
    
    # 链接 Windows 库
    set(PLATFORM_LIBS 
        opengl32
        gdi32
        user32
        kernel32
        ws2_32
        winmm
    )
elseif(APPLE)
    # macOS 特定设置
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
    find_library(COREAUDIO_LIBRARY CoreAudio)
    
    set(PLATFORM_LIBS
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
        ${COREAUDIO_LIBRARY}
    )
else()
    # Linux 特定设置
    find_package(X11 REQUIRED)
    find_package(ALSA QUIET)
    find_package(PulseAudio QUIET)
    
    set(PLATFORM_LIBS
        ${X11_LIBRARIES}
        ${ALSA_LIBRARIES}
        ${PULSEAUDIO_LIBRARIES}
    )
endif()

# 添加子目录
add_subdirectory(src)

# 启用测试
enable_testing()
add_subdirectory(tests) 