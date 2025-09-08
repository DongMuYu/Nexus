#!/bin/bash

# Nexus 物理引擎构建脚本

echo "Building Nexus Physics Engine..."

# 创建构建目录
mkdir -p ../build
cd ../build

# 运行CMake配置
echo "Running CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release

# 编译项目
echo "Compiling project..."
cmake --build . --config Release

echo "Build completed successfully!"
echo "Executable: build/bin/Nexus"