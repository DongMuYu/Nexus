#include "UIManager.h"
#include "../particles/Emitters/MouseEmitterController.h"
#include <iostream>

UIManager::UIManager(sf::RenderWindow& window, ParticleSystem& particleSystem)
    : window(window)
    , particleSystem(particleSystem)
    , showHelp(true)
    , showStats(true)
    , selectedParticle(nullptr)
    , fontLoaded(false) {
    
    // 尝试加载默认字体
    std::string fontPath = "C:/Windows/Fonts/arial.ttf";
    if (!defaultFont.loadFromFile(fontPath)) {
        std::cout << "Warning: Failed to load default font, UI text may not be displayed" << std::endl;
        fontLoaded = false;
    } else {
        fontLoaded = true;
    }
    
    // 初始化UI元素
    initializeUIElements();
    
    std::cout << "UI manager initialized" << std::endl;
}

UIManager::~UIManager() {
    // 析构函数不需要特殊处理
}

void UIManager::initializeUIElements() {
    // Initialize help text
    if (fontLoaded) {
        helpText.setFont(defaultFont);
    } else {
        // If font loading failed, we can't set a font (SFML 2.5.1 doesn't have getDefaultFont)
        helpText.setCharacterSize(12); // Set a reasonable default size
    }
    helpText.setCharacterSize(14);
    helpText.setFillColor(sf::Color::White);
    helpText.setString(
        "Nexus particle system control:\n"
        "Left click: Create particle\n"
        "Left drag: Create particle and set initial velocity\n"
        "Right click: Select particle\n"
        "H: Show/Hide help\n"
        "S: Show/Hide statistics\n"
        "C: Show/Hide particle connections\n"
        "V: Show/Hide velocity vectors\n"
        "F: Show/Hide force vectors\n"
        "1-4: Switch rendering modes\n"
        "ESC: Exit program"
    );
    helpText.setPosition(10, 10);
    
    // 初始化统计文本
    if (fontLoaded) {
        statsText.setFont(defaultFont);
    } else {
        // If font loading failed, we can't set a font (SFML 2.5.1 doesn't have getDefaultFont)
        statsText.setCharacterSize(12); // Set a reasonable default size
    }
    statsText.setCharacterSize(14);
    statsText.setFillColor(sf::Color::White);
    statsText.setPosition(10, 180);
    
    // 初始化粒子信息文本
    if (fontLoaded) {
        particleInfoText.setFont(defaultFont);
    } else {
        // If font loading failed, we can't set a font (SFML 2.5.1 doesn't have getDefaultFont)
        particleInfoText.setCharacterSize(12); // Set a reasonable default size
    }
    particleInfoText.setCharacterSize(12);
    particleInfoText.setFillColor(sf::Color::Yellow);
    particleInfoText.setPosition(window.getSize().x - 250, 10);
}

void UIManager::handleEvent(const sf::Event& event) {
    // 处理键盘事件
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
            case sf::Keyboard::H:
                showHelp = !showHelp;
                break;
            case sf::Keyboard::S:
                showStats = !showStats;
                break;
            case sf::Keyboard::C:
                // 切换粒子连接显示
                // 这里需要通过Application访问Renderer
                break;
            case sf::Keyboard::V:
                // 切换速度向量显示
                break;
            case sf::Keyboard::F:
                // 切换力向量显示
                break;
            case sf::Keyboard::Num1:
                // 切换到默认渲染模式
                break;
            case sf::Keyboard::Num2:
                // 切换到速度渲染模式
                break;
            case sf::Keyboard::Num3:
                // 切换到力渲染模式
                break;
            case sf::Keyboard::Num4:
                // 切换到连接渲染模式
                break;
            default:
                break;
        }
    }
    
    // 处理鼠标事件
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Right) {
            // 右键点击选择粒子
            selectParticleAtPosition(sf::Vector2f(event.mouseButton.x, event.mouseButton.y));
        }
    }
}

void UIManager::update(float deltaTime) {
    // 静态帧计数器，大幅减少更新频率
    static int frameCount = 0;
    const int updateInterval = 60; // 每60帧更新一次（进一步减少更新频率）
    
    // 静态时间累加器，基于时间而不是帧数进行更新
    static float timeAccumulator = 0.0f;
    const float updateTime = 1.0f; // 每1秒更新一次
    
    frameCount++;
    timeAccumulator += deltaTime;
    
    // 使用帧数和时间双重控制，确保在任何情况下都能减少更新频率
    if (frameCount >= updateInterval || timeAccumulator >= updateTime) {
        frameCount = 0;
        timeAccumulator = 0.0f;
        
        // 更新统计信息
        updateStats();
        
        // 更新选中粒子的信息
        if (selectedParticle) {
            updateParticleInfo();
        }
    }
}

void UIManager::render(sf::RenderWindow& window) {
    // 渲染帮助文本（如果启用）
    if (showHelp && fontLoaded) {
        window.draw(helpText);
    }
    
    // 渲染统计信息（如果启用）
    if (showStats && fontLoaded) {
        window.draw(statsText);
    }
    
    // 渲染选中粒子的信息（如果有选中的粒子）
    if (selectedParticle && fontLoaded) {
        window.draw(particleInfoText);
    }
    
    // 渲染其他UI元素
    renderAdditionalUI(window);
}

void UIManager::updateStats() {
    // 获取粒子系统统计信息
    int particleCount = particleSystem.getParticleCount();
    
    // 静态变量用于缓存粒子数量和连接数量
    static int cachedParticleCount = -1;
    static int cachedBondCount = -1;
    static int updateCounter = 0;
    
    // 每60次更新才重新计算一次连接数量，进一步减少计算频率
    updateCounter++;
    // if (updateCounter >= 60 || cachedParticleCount == -1) {
    //     updateCounter = 0;
    //     cachedParticleCount = particleCount;
        
    //     // 计算连接数量 - 大幅优化：减少计算频率和计算量
    //     int bondCount = 0;
    //     const auto& particles = particleSystem.getParticles();
        
    //     // 大幅减少检查的粒子数量
    //     size_t maxParticlesToCheck = std::min(particles.size(), static_cast<size_t>(50));
        
    //     if (particles.size() > maxParticlesToCheck) {
    //         // 如果粒子数量超过限制，使用更简单的估算方法
    //         // 假设平均每个粒子有2-3个连接
    //         bondCount = static_cast<int>(particleCount * 2.5);
    //     } else {
    //         // 如果粒子数量很少，正常计算
    //         for (size_t i = 0; i < particles.size(); ++i) {
    //             bondCount += particles[i]->getBondCount();
    //         }
    //     }
        
    //     cachedBondCount = bondCount / 2; // Divided by 2 because each bond is counted twice
    // }
    
    // Update statistics text
    std::string statsString = "Statistics:\n";
    statsString += "Particle count: " + std::to_string(particleCount) + "\n";
    
    // 计算平均速度
    // 注意：velocity是受保护的成员，无法直接访问
    statsString += "Average velocity: (Cannot access directly)\n";
    
    // 计算总动能
    // 注意：velocity是受保护的成员，无法直接访问
    statsString += "Total kinetic energy: (Cannot access directly)\n";
    
    // 使用缓存的连接数量
    statsString += "Bond count: " + std::to_string(cachedBondCount) + "\n";
    
    statsText.setString(statsString);
}

void UIManager::updateParticleInfo() {
    // 静态变量用于缓存选中的粒子指针和上一次的信息字符串
    static CoreParticle* cachedSelectedParticle = nullptr;
    static std::string cachedInfoString = "No particle selected";
    
    // 只有当选中粒子发生变化时才更新信息
    CoreParticle* currentParticle = selectedParticle ? selectedParticle.get() : nullptr;
    if (cachedSelectedParticle != currentParticle) {
        cachedSelectedParticle = currentParticle;
        
        if (selectedParticle) {
            // 获取选中粒子的信息
            Vector2 position = selectedParticle->getPosition();
            // 注意：velocity是受保护的成员，无法直接访问
            // Vector2 velocity = selectedParticle->getVelocity();
            // 注意：forceAccumulator是受保护的成员，无法直接访问
            // Vector2 force = selectedParticle->getForceAccumulator();
            float mass = selectedParticle->getMass();
            float radius = selectedParticle->getRadius();
            // int bonds = selectedParticle->getBondCount();
            
            // Update particle information text
            std::string infoString = "Selected particle info:\n";
            infoString += "Position: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")\n";
            infoString += "Velocity: (Cannot access directly)\n";
              infoString += "Speed: (Cannot access directly)\n";
            infoString += "Force: (Cannot access directly)\n";
            infoString += "Mass: " + std::to_string(mass) + "\n";
            infoString += "Radius: " + std::to_string(radius) + "\n";
            // infoString += "Bond count: " + std::to_string(bonds) + "\n";
            
            cachedInfoString = infoString;
        } else {
            cachedInfoString = "No particle selected";
        }
    }
    
    particleInfoText.setString(cachedInfoString);
}

void UIManager::selectParticleAtPosition(sf::Vector2f position) {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 重置选中粒子
    selectedParticle = nullptr;
    
    // 如果没有粒子，直接返回
    if (particles.empty()) {
        return;
    }
    
    // 查找点击位置最近的粒子
    float minDistance = std::numeric_limits<float>::max();
    
    // 优化：只检查点击位置附近一定范围内的粒子
    float searchRadius = 30.0f; // 减小搜索半径
    float searchRadiusSquared = searchRadius * searchRadius;
    
    // 大幅限制检查的粒子数量，避免性能问题
    size_t maxParticlesToCheck = std::min(particles.size(), static_cast<size_t>(100));
    
    // 使用简单的采样方法，而不是遍历所有粒子
    size_t step = particles.size() > maxParticlesToCheck ? particles.size() / maxParticlesToCheck : 1;
    
    for (size_t i = 0; i < particles.size(); i += step) {
        Vector2 particlePos = particles[i]->getPosition();
        
        // 首先进行粗略的距离检查，避免计算平方根
        float dx = position.x - particlePos.x;
        float dy = position.y - particlePos.y;
        float distanceSquared = dx * dx + dy * dy;
        
        // 如果粒子在搜索半径内
        if (distanceSquared <= searchRadiusSquared) {
            float distance = std::sqrt(distanceSquared);
            float particleRadius = particles[i]->getRadius();
            
            // 如果点击位置在粒子范围内
            if (distance <= particleRadius && distance < minDistance) {
                minDistance = distance;
                selectedParticle = particles[i];
            }
        }
    }
}

void UIManager::renderAdditionalUI(sf::RenderWindow& window) {
    // 渲染选中粒子的高亮
    if (selectedParticle) {
        Vector2 position = selectedParticle->getPosition();
        float radius = selectedParticle->getRadius();
        
        // 创建高亮圆圈
        sf::CircleShape highlight(radius + 3);
        highlight.setOrigin(radius + 3, radius + 3);
        highlight.setPosition(position.x, position.y);
        highlight.setFillColor(sf::Color::Transparent);
        highlight.setOutlineColor(sf::Color::Yellow);
        highlight.setOutlineThickness(2.0f);
        
        // 绘制高亮
        window.draw(highlight);
    }
}

// 设置方法实现
void UIManager::setShowHelp(bool show) {
    showHelp = show;
}

void UIManager::setShowStats(bool show) {
    showStats = show;
}

void UIManager::setSelectedParticle(std::shared_ptr<CoreParticle> particle) {
    selectedParticle = particle;
}

// 获取方法实现
bool UIManager::getShowHelp() const {
    return showHelp;
}

bool UIManager::getShowStats() const {
    return showStats;
}

std::shared_ptr<CoreParticle> UIManager::getSelectedParticle() const {
    return selectedParticle;
}

bool UIManager::isFontLoaded() const {
    return fontLoaded;
}