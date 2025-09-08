#!/bin/bash

# Nexus 物理引擎运行脚本

echo "Starting Nexus Physics Engine..."

# 检查构建目录是否存在
if [ ! -d "../build" ]; then
    echo "Build directory not found. Please run build.sh first."
    exit 1
fi

# 运行可执行文件
cd ../build
echo "Running Nexus..."
./bin/Nexus

echo "Nexus has exited."