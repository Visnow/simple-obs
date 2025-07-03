# Building SimpleOBS

## Prerequisites

### Required Software
- **CMake**: 3.16 or higher
- **C++ Compiler**: Supporting C++17 standard
  - Windows: Visual Studio 2019 or later
  - Linux: GCC 7+ or Clang 6+
  - macOS: Xcode 10+ or Clang 6+
- **OpenGL**: Development libraries
- **Threading**: Standard threading library

### Optional Dependencies
- **Ninja**: Faster build system (recommended)
- **Git**: For version control

## Building on Windows

### Using Visual Studio
```bash
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

### Using Ninja (Recommended)
```bash
mkdir build
cd build
cmake .. -G Ninja
ninja
```

### Using CMake Presets
```bash
# Configure and build in one step
cmake --preset default
cmake --build --preset default
```

## Building on Linux

### Ubuntu/Debian
```bash
# Install dependencies
sudo apt update
sudo apt install build-essential cmake ninja-build libgl1-mesa-dev

# Build
mkdir build && cd build
cmake .. -G Ninja
ninja
```

### CentOS/RHEL/Fedora
```bash
# Install dependencies
sudo dnf install gcc-c++ cmake ninja-build mesa-libGL-devel

# Build
mkdir build && cd build
cmake .. -G Ninja
ninja
```

## Building on macOS

### Using Homebrew
```bash
# Install dependencies
brew install cmake ninja

# Build
mkdir build && cd build
cmake .. -G Ninja
ninja
```

### Using Xcode
```bash
mkdir build
cd build
cmake .. -G Xcode
cmake --build . --config Release
```

## Build Options

### CMake Variables
```bash
# Set build type
cmake .. -DCMAKE_BUILD_TYPE=Release

# Enable tests
cmake .. -DBUILD_TESTS=ON

# Set installation prefix
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local
```

### Build Types
- **Debug**: Full debug information, no optimizations
- **Release**: Optimized for performance
- **RelWithDebInfo**: Optimized with debug information
- **MinSizeRel**: Optimized for minimum size

## Running the Application

After successful build:
```bash
# Windows
./build/bin/Release/SimpleOBS.exe

# Linux/macOS
./build/bin/SimpleOBS
```

## Troubleshooting

### Common Issues

1. **CMake not found**
   - Install CMake from https://cmake.org/download/
   - Ensure it's in your PATH

2. **Compiler not found**
   - Install appropriate compiler for your platform
   - Ensure it's in your PATH

3. **OpenGL not found**
   - Windows: Install graphics drivers
   - Linux: Install `libgl1-mesa-dev` or equivalent
   - macOS: Install Xcode command line tools

4. **Build fails with C++17 errors**
   - Update your compiler to support C++17
   - Check compiler version: `g++ --version` or `clang++ --version`

### Getting Help
- Check the build output for specific error messages
- Ensure all prerequisites are installed
- Try building with verbose output: `cmake --build . --verbose` 