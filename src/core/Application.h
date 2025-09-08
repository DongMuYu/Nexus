// src/core/Application.h
// 应用程序主类，负责管理整个粒子系统的运行
#pragma once
#include <SFML/Graphics.hpp>
#include "../particles/ParticleSystem.h"
#include "../rendering/Renderer.h"
#include "../ui/UIManager.h"
#include "../particles/Emitters/MouseEmitterController.h"

class Application {
private:
    // SFML渲染窗口，用于显示图形界面
    sf::RenderWindow window;
    
    // 时钟，用于计算帧间隔时间
    sf::Clock clock;
    
    // 每帧的时间间隔
    sf::Time deltaTime;
    
    // 时间缩放因子，用于控制模拟速度
    float timeScale;
    
    // 粒子系统，管理所有粒子的创建、更新和销毁
    ParticleSystem particleSystem;
    
    // 渲染器，负责将粒子系统绘制到窗口上
    Renderer renderer;
    
    // UI管理器，处理用户界面和交互
    UIManager uiManager;
    
    // 鼠标发射器控制器，处理鼠标相关的粒子发射
    std::shared_ptr<MouseEmitterController> mouseEmitter;
    
    // 应用程序运行状态标志
    bool isRunning;
    
    // 应用程序暂停状态标志
    bool isPaused;
    
    // 窗口拖动状态标志
    bool isWindowDragging;
    
    // 窗口的上一帧位置，用于检测窗口是否被拖动
    sf::Vector2i previousWindowPosition;
    
    // FPS计算相关变量
    int frameCount;          // 帧计数器
    float fpsAccumulator;    // FPS累加器
    int currentFPS;         // 当前FPS值
    
public:
    // 构造函数，初始化应用程序
    // 参数：width - 窗口宽度，height - 窗口高度，title - 窗口标题
    Application(int width = 1200, int height = 1200, const std::string& title = "Nexus Physics");
    
    // 析构函数，清理资源
    ~Application();
    
    // 运行应用程序主循环
    void run();
    
    // 处理用户输入事件
    void handleEvents();
    
    // 更新应用程序状态
    // 参数：deltaTime - 距离上一帧的时间（秒）
    void update(float deltaTime);
    
    // 渲染应用程序画面
    void render();
    
    // 计算并更新FPS
    // 参数：elapsedTime - 经过的时间
    void calculateFPS(sf::Time elapsedTime);
    
    // 获取应用程序运行状态
    bool isAppRunning() const { return isRunning; }
    
    // 获取经过的时间（考虑时间缩放）
    float getDeltaTime() const { return deltaTime.asSeconds() * timeScale; }
    
    // 获取当前FPS
    float getFPS() const { return 1.0f / deltaTime.asSeconds(); }
    
    // 获取渲染窗口引用
    sf::RenderWindow& getWindow() { return window; }
    
    // 获取粒子系统引用
    ParticleSystem& getParticleSystem() { return particleSystem; }
    
    // 获取渲染器引用
    Renderer& getRenderer() { return renderer; }
    
    // 获取UI管理器引用
    UIManager& getUIManager() { return uiManager; }
    
    // 时间缩放相关方法
    // 设置时间缩放因子
    // 参数：scale - 缩放因子，1.0为正常速度，>1.0为加速，<1.0为减速
    void setTimeScale(float scale);
    
    // 获取当前时间缩放因子
    float getTimeScale() const;
    
    // 窗口拖动状态相关方法
    // 获取窗口是否正在被拖动
    bool isWindowBeingDragged() const { return isWindowDragging; }
    
    // 设置窗口拖动状态
    void setWindowDragging(bool dragging) { isWindowDragging = dragging; isPaused = dragging; }
    
    // 获取应用程序是否暂停
    bool isAppPaused() const { return isPaused; }
    
    // 设置应用程序暂停状态
    void setPaused(bool paused) { isPaused = paused; }
};