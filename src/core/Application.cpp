#include "Application.h"
// 引入鼠标发射器控制器头文件
#include "../particles/Emitters/MouseEmitterController.h"
// 引入输入输出流库，用于控制台输出
#include <iostream>

/**
 * @brief 应用程序构造函数
 * @param width 窗口宽度（像素）
 * @param height 窗口高度（像素）
 * @param title 窗口标题
 */
Application::Application(int width, int height, const std::string& title)
    // 初始化SFML渲染窗口，设置指定尺寸和标题
    : window(sf::VideoMode(width, height), title)
    // 初始化粒子系统
    , particleSystem()
    // 初始化渲染器，传入窗口和粒子系统引用
    , renderer(window, particleSystem)
    // 初始化UI管理器，传入窗口和粒子系统引用
    , uiManager(window, particleSystem)
    // 设置应用程序运行状态为true
    , isRunning(true)
    // 设置应用程序暂停状态为false
    , isPaused(false)
    // 设置窗口拖动状态为false
    , isWindowDragging(false)
    // 初始化窗口位置为(0,0)
    , previousWindowPosition(0, 0)
    // 设置时间缩放因子为1.0（正常速度）
    , timeScale(1.0f) {
    
    // 设置窗口属性
    window.setFramerateLimit(60); // 限制帧率为60FPS，避免过高帧率消耗过多资源
    window.setVerticalSyncEnabled(true); // 启用垂直同步，防止画面撕裂
    
    // 初始化粒子系统
    particleSystem.setSpatialPartitioning(true); // 启用空间分区，提高大量粒子时的性能
    particleSystem.setSceneSize(static_cast<float>(width), static_cast<float>(height)); // 设置场景大小为窗口大小
    
    // 初始化鼠标发射器控制器
    mouseEmitter = std::make_shared<MouseEmitterController>(window);
    
    // 输出初始化完成信息到控制台
    std::cout << "Application initialized" << std::endl;
}

/**
 * @brief 应用程序析构函数
 */
Application::~Application() {
    // 输出应用程序关闭信息到控制台
    std::cout << "Application closing" << std::endl;
}

/**
 * @brief 应用程序主循环
 * 
 * 该方法包含应用程序的主循环，负责处理事件、更新游戏状态和渲染画面。
 * 使用固定时间步长更新确保游戏逻辑在不同帧率下表现一致。
 */
void Application::run() {
    // 创建SFML时钟用于计算帧时间
    sf::Clock clock;
    // 自上次更新以来的时间累计
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    // 每帧的目标时间（120FPS）
    const sf::Time timePerFrame = sf::seconds(1.f / 300.f); // 300 FPS
    
    // 主循环，当应用程序运行且窗口打开时继续
    while (isRunning && window.isOpen()) {
        // 处理所有待处理的事件（如键盘、鼠标输入等）
        handleEvents();
        
        // 固定时间步长更新
        // 获取自上一帧以来的经过时间
        sf::Time elapsedTime = clock.restart();
        // 累计时间
        timeSinceLastUpdate += elapsedTime;
        
        // 如果累计时间超过一帧的时间，则进行更新
        // 这确保了即使帧率波动，游戏逻辑更新频率也保持稳定
        while (timeSinceLastUpdate > timePerFrame) {
            timeSinceLastUpdate -= timePerFrame;
            // 更新游戏状态，传入帧时间和时间缩放因子
            update(timePerFrame.asSeconds() * timeScale);
        }
        
        // 渲染画面
        render();
        
        // 计算并显示FPS
        calculateFPS(elapsedTime);
    }
}

/**
 * @brief 处理窗口和输入事件
 * 
 * 该方法负责处理所有窗口事件和用户输入，包括窗口关闭、键盘输入、
 * 窗口焦点变化等。当窗口被拖动时，会暂停游戏更新以节省资源。
 */
void Application::handleEvents() {
    // 创建事件对象
    sf::Event event;
    // 轮询处理所有待处理的事件
    while (window.pollEvent(event)) {
        // 处理窗口关闭事件
        if (event.type == sf::Event::Closed) {
            // 设置运行状态为false，退出主循环
            isRunning = false;
            // 关闭窗口
            window.close();
        }
        // 处理ESC键退出
        else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
            // 设置运行状态为false，退出主循环
            isRunning = false;
            // 关闭窗口
            window.close();
        }
        // 处理窗口失去焦点事件（窗口被拖动时会触发）
        else if (event.type == sf::Event::LostFocus) {
            // 设置窗口拖动状态为true
            isWindowDragging = true;
            // 设置应用程序暂停状态为true
            isPaused = true;
            // 设置粒子系统暂停状态为true
            particleSystem.setPaused(true);
            // 设置渲染器暂停状态为true
            renderer.setPaused(true);
            // 设置鼠标发射器暂停状态为true
            mouseEmitter->setPaused(true);
            // 输出调试信息
            std::cout << "[DEBUG] Window lost focus - pausing updates" << std::endl;
            std::cout << "[DEBUG] isWindowDragging: " << (isWindowDragging ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] isPaused: " << (isPaused ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] particleSystem.isPaused: " << (particleSystem.isSystemPaused() ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] renderer.isPaused: " << (renderer.isRendererPaused() ? "true" : "false") << std::endl;
        }
    
        // 处理鼠标按键释放事件（窗口拖动结束时会触发）
        else if (event.type == sf::Event::MouseButtonReleased) {
            // 如果正在拖动窗口，现在停止拖动
            if (isWindowDragging) {
                // 设置窗口拖动状态为false
                isWindowDragging = false;
                // 设置应用程序暂停状态为false
                isPaused = false;
                // 设置粒子系统暂停状态为false
                particleSystem.setPaused(false);
                // 设置渲染器暂停状态为false
                renderer.setPaused(false);
                // 设置鼠标发射器控制器暂停状态为false
                mouseEmitter->setPaused(false);
                // 输出调试信息
                std::cout << "[DEBUG] Mouse button released - resuming updates" << std::endl;
                std::cout << "[DEBUG] isWindowDragging: " << (isWindowDragging ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] isPaused: " << (isPaused ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] particleSystem.isPaused: " << (particleSystem.isSystemPaused() ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] renderer.isPaused: " << (renderer.isRendererPaused() ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] mouseEmitter.isPaused: " << (mouseEmitter->isControllerPaused() ? "true" : "false") << std::endl;
            }
        }
        // 处理窗口获得焦点事件（窗口拖动结束时会触发）
        else if (event.type == sf::Event::GainedFocus) {
            // 设置窗口拖动状态为false
            isWindowDragging = false;
            // 设置应用程序暂停状态为false
            isPaused = false;
            // 设置粒子系统暂停状态为false
            particleSystem.setPaused(false);
            // 设置渲染器暂停状态为false
            renderer.setPaused(false);
            // 设置鼠标发射器控制器暂停状态为false
            mouseEmitter->setPaused(false);
            // 输出调试信息
            std::cout << "[DEBUG] Window gained focus - resuming updates" << std::endl;
            std::cout << "[DEBUG] isWindowDragging: " << (isWindowDragging ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] isPaused: " << (isPaused ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] particleSystem.isPaused: " << (particleSystem.isSystemPaused() ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] renderer.isPaused: " << (renderer.isRendererPaused() ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] mouseEmitter.isPaused: " << (mouseEmitter->isControllerPaused() ? "true" : "false") << std::endl;
        }
        
        // 只有在窗口没有被拖动时才处理UI和鼠标事件
        if (!isWindowDragging) {
            // 将事件传递给UI管理器处理
            uiManager.handleEvent(event);
            
            // 传递事件给鼠标发射器
            // 获取粒子系统的粒子列表（需要const_cast因为getParticles()返回const引用）
            auto& particles = const_cast<std::vector<std::shared_ptr<CoreParticle>>&>(particleSystem.getParticles());
            // 让鼠标发射器处理事件
            mouseEmitter->handleEvent(event, particles);
        }
        else {
            // 窗口拖动时，跳过所有UI和鼠标事件处理
            // 特别注意：跳过鼠标发射器的事件处理，防止在窗口拖动时创建新的粒子发射器
            std::cout << "[DEBUG] Skipping UI and mouse events during window drag" << std::endl;
        }
    }
}

/**
 * @brief 更新应用程序状态
 * @param deltaTime 自上一帧以来的时间增量（秒）
 * 
 * 该方法负责更新所有游戏对象的状态，包括粒子系统、鼠标发射器和UI。
 * 当窗口被拖动或应用程序暂停时，跳过所有更新以节省资源。
 */
void Application::update(float deltaTime) {
    // 检查窗口位置是否发生变化，以检测窗口是否被拖动
    sf::Vector2i currentWindowPosition = window.getPosition();
    if (currentWindowPosition != previousWindowPosition) {
        // 如果之前没有在拖动，现在开始拖动
        if (!isWindowDragging) {
            // 设置窗口拖动状态为true
            isWindowDragging = true;
            // 设置应用程序暂停状态为true
            isPaused = true;
            // 设置粒子系统暂停状态为true
            particleSystem.setPaused(true);
            // 设置渲染器暂停状态为true
            renderer.setPaused(true);
            // 设置鼠标发射器控制器暂停状态为true
            mouseEmitter->setPaused(true);
            // 输出调试信息
            std::cout << "[DEBUG] Window position changed - pausing updates" << std::endl;
            std::cout << "[DEBUG] isWindowDragging: " << (isWindowDragging ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] isPaused: " << (isPaused ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] particleSystem.isPaused: " << (particleSystem.isSystemPaused() ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] renderer.isPaused: " << (renderer.isRendererPaused() ? "true" : "false") << std::endl;
            std::cout << "[DEBUG] mouseEmitter.isPaused: " << (mouseEmitter->isControllerPaused() ? "true" : "false") << std::endl;
        }
        // 更新窗口位置
        previousWindowPosition = currentWindowPosition;
    }
    
    // 如果窗口正在被拖动或应用程序已暂停，则不更新粒子系统和UI
    // 这有助于在窗口拖动时减少CPU使用率，提高系统响应性
    if (isWindowDragging || isPaused) {
        // 添加调试输出，确认更新被跳过
        static bool debugPrinted = false;
        if (!debugPrinted) {
            std::cout << "[DEBUG] Update skipped due to window dragging or pause" << std::endl;
            debugPrinted = true;
        }
        
        // 检查窗口位置是否在一段时间内没有变化，以检测窗口是否停止拖动
        static sf::Time windowPositionTimer = sf::Time::Zero;
        static sf::Vector2i lastStablePosition = window.getPosition();
        
        // 如果当前位置与上次稳定位置相同，增加计时器
        if (window.getPosition() == lastStablePosition) {
            windowPositionTimer += sf::seconds(deltaTime);
            // 如果窗口位置在0.5秒内没有变化，认为窗口停止拖动
            if (windowPositionTimer >= sf::seconds(0.5f)) {
                // 设置窗口拖动状态为false
                isWindowDragging = false;
                // 设置应用程序暂停状态为false
                isPaused = false;
                // 设置粒子系统暂停状态为false
                particleSystem.setPaused(false);
                // 设置渲染器暂停状态为false
                renderer.setPaused(false);
                // 设置鼠标发射器控制器暂停状态为false
                mouseEmitter->setPaused(false);
                // 输出调试信息
                std::cout << "[DEBUG] Window position stable - resuming updates" << std::endl;
                std::cout << "[DEBUG] isWindowDragging: " << (isWindowDragging ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] isPaused: " << (isPaused ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] particleSystem.isPaused: " << (particleSystem.isSystemPaused() ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] renderer.isPaused: " << (renderer.isRendererPaused() ? "true" : "false") << std::endl;
                std::cout << "[DEBUG] mouseEmitter.isPaused: " << (mouseEmitter->isControllerPaused() ? "true" : "false") << std::endl;
                // 重置计时器
                windowPositionTimer = sf::Time::Zero;
            }
        } else {
            // 窗口位置发生变化，更新稳定位置并重置计时器
            lastStablePosition = window.getPosition();
            windowPositionTimer = sf::Time::Zero;
        }
        
        return;
    }
    
    // 重置调试标志
    static bool debugPrintedResume = false;
    if (!debugPrintedResume) {
        std::cout << "[DEBUG] Update resumed" << std::endl;
        debugPrintedResume = true;
    }
    
    // 更新粒子系统
    // 传入时间增量，确保粒子运动与时间相关而非帧率相关
    particleSystem.update(deltaTime);
    
    // 更新鼠标发射器
    // 获取粒子系统的粒子列表（需要const_cast因为getParticles()返回const引用）
    auto& particles = const_cast<std::vector<std::shared_ptr<CoreParticle>>&>(particleSystem.getParticles());
    // 更新鼠标发射器状态，传入时间增量和粒子列表
    mouseEmitter->update(deltaTime, particles);
    
    // 更新UI管理器
    // 目前暂时注释掉，以测试是否是UIManager导致程序卡顿
    // uiManager.update(deltaTime);
}

/**
 * @brief 渲染应用程序画面
 * 
 * 该方法负责渲染所有可见元素，包括背景、粒子系统、鼠标发射器和UI。
 * 渲染顺序很重要：先渲染背景，然后是粒子系统，接着是鼠标发射器，最后是UI。
 */
void Application::render() {
    // 清空窗口，设置深蓝色背景
    window.clear(sf::Color(20, 20, 30)); // 深蓝色背景
    
    // 如果系统没有暂停，才渲染动态内容
    if (!isWindowDragging && !isPaused) {
        // 渲染粒子系统
        // 调用渲染器的render方法绘制所有粒子
        renderer.render();
        
        // 渲染鼠标发射器
        // 传入窗口引用，让鼠标发射器绘制其可视化元素（如发射器位置等）
        mouseEmitter->render(window);
    } else {
        // 当窗口拖动或暂停时，仅渲染静态背景
        // 不渲染粒子系统和鼠标发射器，避免显示"移动"的错觉
        std::cout << "[DEBUG] Render skipped due to window dragging or pause" << std::endl;
    }
    
    // 始终渲染UI（即使在暂停状态下也显示UI）
    // 传入窗口引用，让UI管理器绘制所有UI元素
    uiManager.render(window);
    
    // 显示内容
    // 将所有渲染的缓冲区内容显示到屏幕上
    window.display();
}

/**
 * @brief 计算并显示FPS（每秒帧数）
 * @param elapsedTime 自上一帧以来的时间增量
 * 
 * 该方法计算当前FPS并更新窗口标题以显示实时帧率。
 * 使用静态变量在方法调用之间保持状态，每秒更新一次FPS显示。
 */
void Application::calculateFPS(sf::Time elapsedTime) {
    // 初始化FPS计算变量
    // 使用静态变量在方法调用之间保持状态
    static bool firstCall = true;
    if (firstCall) {
        // 帧计数器
        frameCount = 0;
        // 时间累计器（秒）
        fpsAccumulator = 0.0f;
        // 当前FPS值
        currentFPS = 0;
        // 标记初始化完成
        firstCall = false;
    }
    // 增加帧计数
    frameCount++;
    // 累计时间
    fpsAccumulator += elapsedTime.asSeconds();
    
    // 每秒计算一次FPS
    if (fpsAccumulator >= 1.0f) {
        // 计算FPS：帧数除以时间（秒）
        currentFPS = static_cast<int>(frameCount / fpsAccumulator);
        // 重置帧计数器
        frameCount = 0;
        // 重置时间累计器
        fpsAccumulator = 0;
        
        // 更新窗口标题显示FPS
        // 将FPS值转换为字符串并添加到窗口标题
        window.setTitle("Nexus Particle System - FPS: " + std::to_string(currentFPS));
    }
}

/**
 * @brief 设置时间缩放因子
 * @param scale 时间缩放因子（1.0为正常速度，<1.0为慢动作，>1.0为快动作）
 * 
 * 时间缩放因子影响游戏中的时间流逝速度，可用于实现慢动作或快进效果。
 */
void Application::setTimeScale(float scale) {
    // 设置时间缩放因子
    timeScale = scale;
}

/**
 * @brief 获取当前时间缩放因子
 * @return 当前的时间缩放因子
 */
float Application::getTimeScale() const {
    // 返回当前的时间缩放因子
    return timeScale;
}