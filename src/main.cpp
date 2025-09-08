// src/main.cpp
#include "core/Application.h"
#include "particles/CoreParticle.h"
#include "rendering/Renderer.h"
#include <memory>

int main() {
    // 创建应用程序实例
    // 窗口大小：1200x1200，标题："Nexus粒子系统"
    Application app(1200, 1200, "Nexus粒子系统");
    
    // 运行应用程序主循环
    app.run();
    
    return 0;
}