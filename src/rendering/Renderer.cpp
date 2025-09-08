#include "Renderer.h"
#include "../particles/CoreParticle.h"
#include "../particles/Emitters/BaseEmitter.h"
#include <iostream>
#include <algorithm>

Renderer::Renderer(sf::RenderWindow& window, ParticleSystem& particleSystem)
    : window(window)
    , particleSystem(particleSystem)
    , showParticleConnections(true)
    , showParticleTrails(false)
    , showVelocityVectors(false)
    , showForceVectors(false)
    , renderMode(RenderMode::Default)
    , isPaused(false) {
    
    // 初始化渲染设置
    initializeRenderSettings();
    
    std::cout << "Renderer initialized" << std::endl;
}

void Renderer::initializeRenderSettings() {
    // 设置默认粒子颜色渐变
    particleGradient.addColorPoint(0.0f, sf::Color(255, 100, 100)); // 红色
    particleGradient.addColorPoint(0.5f, sf::Color(100, 255, 100)); // 绿色
    particleGradient.addColorPoint(1.0f, sf::Color(100, 100, 255)); // 蓝色
    
    // 设置连接线颜色
    connectionColor = sf::Color(150, 150, 255, 100); // 半透明蓝色
    
    // 设置向量颜色
    velocityVectorColor = sf::Color(255, 255, 0, 150); // 半透明黄色
    forceVectorColor = sf::Color(255, 0, 0, 150);     // 半透明红色
    
    // 设置背景颜色
    backgroundColor = sf::Color(20, 20, 30); // 深蓝色
}

void Renderer::render() {
    // 检查是否暂停渲染
    if (isPaused) {
        std::cout << "[DEBUG] Renderer render skipped due to pause" << std::endl;
        return;
    }
    
    // 清空窗口
    window.clear(backgroundColor);
    
    // 根据渲染模式选择渲染方法
    switch (renderMode) {
        case RenderMode::Default:
            renderDefault();
            break;
        case RenderMode::Velocity:
            renderByVelocity();
            break;
        case RenderMode::Force:
            renderByForce();
            break;
        case RenderMode::Connections:
            renderConnections();
            break;
    }
    
    // 渲染粒子连接（如果启用）
    if (showParticleConnections) {
        renderParticleConnections();
    }
    
    // 渲染速度向量（如果启用）
    if (showVelocityVectors) {
        renderVelocityVectors();
    }
    
    // 渲染力向量（如果启用）
    if (showForceVectors) {
        renderForceVectors();
    }
}

void Renderer::renderDefault() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 渲染每个粒子
    for (const auto& particle : particles) {
        renderParticle(particle);
    }
}

void Renderer::renderByVelocity() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 注意：velocity是受保护的成员，无法直接访问
    // 如果需要实现按速度渲染，需要在CoreParticle类中添加公共的getVelocity方法
    
    // 暂时使用默认渲染方式
    renderDefault();
}

void Renderer::renderByForce() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 注意：forceAccumulator是受保护的成员，无法直接访问
    // 如果需要实现按力渲染，需要在CoreParticle类中添加公共的getForceAccumulator方法
    
    // 暂时使用默认渲染方式
    renderDefault();
}

void Renderer::renderConnections() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 渲染每个粒子
    for (const auto& particle : particles) {
        renderParticle(particle);
    }
    
    // 渲染粒子间的连接
    renderParticleConnections();
}

void Renderer::renderParticle(std::shared_ptr<CoreParticle> particle, sf::Color color) {
    // 获取粒子属性
    Vector2 position = particle->getPosition();
    float radius = particle->getRadius();
    // 注意：CoreParticle类没有getRotation方法
    // float rotation = particle->getRotation();
    
    // 创建粒子形状
    sf::CircleShape shape(radius);
    shape.setOrigin(radius, radius); // 设置原点为中心
    shape.setPosition(position.x, position.y);
    // shape.setRotation(rotation);
    
    // 设置颜色和透明度
    if (color == sf::Color::White) {
        // 如果没有指定颜色，使用粒子自身的颜色
        shape.setFillColor(particle->getColor());
    } else {
        shape.setFillColor(color);
    }
    
    // 设置轮廓
    shape.setOutlineThickness(1.0f);
    shape.setOutlineColor(sf::Color(255, 255, 255, 100));
    
    // 绘制粒子
    window.draw(shape);
    
    // 注意：CoreParticle类没有纹理相关的方法
    // 如果需要支持纹理，需要在CoreParticle类中添加相应的方法
}

void Renderer::renderParticleConnections() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 注意：bonds是受保护的成员，无法直接访问
    // 如果需要实现连接渲染，需要在CoreParticle类中添加公共的getBonds方法
    
    // 暂时不渲染任何内容
}

void Renderer::renderVelocityVectors() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 注意：velocity是受保护的成员，无法直接访问
    // 如果需要实现速度向量渲染，需要在CoreParticle类中添加公共的getVelocity方法
    
    // 暂时不渲染任何内容
}

void Renderer::renderForceVectors() {
    // 获取所有粒子
    const auto& particles = particleSystem.getParticles();
    
    // 注意：forceAccumulator是受保护的成员，无法直接访问
    // 如果需要实现力向量渲染，需要在CoreParticle类中添加公共的getForceAccumulator方法
    
    // 暂时不渲染任何内容
}

void Renderer::setPaused(bool paused) {
    isPaused = paused;
    if (paused) {
        std::cout << "[DEBUG] Renderer paused" << std::endl;
    } else {
        std::cout << "[DEBUG] Renderer resumed" << std::endl;
    }
}

bool Renderer::isRendererPaused() const {
    return isPaused;
}

void Renderer::drawArrowHead(Vector2 start, Vector2 end, sf::Color color) {
    // 计算向量方向
    Vector2 direction = end - start;
    float length = direction.length();
    
    // 如果向量太短，不绘制箭头
    if (length < 5.0f) return;
    
    // 归一化方向向量
    direction = direction.normalized();
    
    // 计算垂直向量
    Vector2 perpendicular(-direction.y, direction.x);
    
    // 箭头大小
    float arrowSize = std::min(10.0f, length * 0.3f);
    
    // 计算箭头点
    Vector2 arrowPoint1 = end - direction * arrowSize + perpendicular * (arrowSize * 0.5f);
    Vector2 arrowPoint2 = end - direction * arrowSize - perpendicular * (arrowSize * 0.5f);
    
    // 创建箭头三角形
    sf::ConvexShape arrowHead(3);
    arrowHead.setPoint(0, sf::Vector2f(end.x, end.y));
    arrowHead.setPoint(1, sf::Vector2f(arrowPoint1.x, arrowPoint1.y));
    arrowHead.setPoint(2, sf::Vector2f(arrowPoint2.x, arrowPoint2.y));
    arrowHead.setFillColor(color);
    
    // 绘制箭头
    window.draw(arrowHead);
}

// 设置方法实现
void Renderer::setShowParticleConnections(bool show) {
    showParticleConnections = show;
}

void Renderer::setShowParticleTrails(bool show) {
    showParticleTrails = show;
}

void Renderer::setShowVelocityVectors(bool show) {
    showVelocityVectors = show;
}

void Renderer::setShowForceVectors(bool show) {
    showForceVectors = show;
}

void Renderer::setRenderMode(RenderMode mode) {
    renderMode = mode;
}

void Renderer::setBackgroundColor(sf::Color color) {
    backgroundColor = color;
}

void Renderer::setConnectionColor(sf::Color color) {
    connectionColor = color;
}

// 获取方法实现
bool Renderer::getShowParticleConnections() const {
    return showParticleConnections;
}

bool Renderer::getShowParticleTrails() const {
    return showParticleTrails;
}

bool Renderer::getShowVelocityVectors() const {
    return showVelocityVectors;
}

bool Renderer::getShowForceVectors() const {
    return showForceVectors;
}

Renderer::RenderMode Renderer::getRenderMode() const {
    return renderMode;
}

sf::Color Renderer::getBackgroundColor() const {
    return backgroundColor;
}

sf::Color Renderer::getConnectionColor() const {
    return connectionColor;
}

// 颜色渐变类方法实现
void Renderer::ColorGradient::addColorPoint(float position, sf::Color color) {
    colorPoints.push_back({position, color});
    // 按位置排序
    std::sort(colorPoints.begin(), colorPoints.end(), 
        [](const ColorPoint& a, const ColorPoint& b) {
            return a.position < b.position;
        });
}

sf::Color Renderer::ColorGradient::getColor(float position) const {
    // 如果没有颜色点，返回白色
    if (colorPoints.empty()) {
        return sf::Color::White;
    }
    
    // 如果位置超出范围，返回边界颜色
    if (position <= colorPoints.front().position) {
        return colorPoints.front().color;
    }
    
    if (position >= colorPoints.back().position) {
        return colorPoints.back().color;
    }
    
    // 找到位置所在的两个颜色点
    for (size_t i = 0; i < colorPoints.size() - 1; i++) {
        if (position >= colorPoints[i].position && position <= colorPoints[i + 1].position) {
            // 计算插值因子
            float factor = (position - colorPoints[i].position) / 
                          (colorPoints[i + 1].position - colorPoints[i].position);
            
            // 线性插值
            sf::Color color;
            color.r = static_cast<sf::Uint8>(colorPoints[i].color.r + 
                          factor * (colorPoints[i + 1].color.r - colorPoints[i].color.r));
            color.g = static_cast<sf::Uint8>(colorPoints[i].color.g + 
                          factor * (colorPoints[i + 1].color.g - colorPoints[i].color.g));
            color.b = static_cast<sf::Uint8>(colorPoints[i].color.b + 
                          factor * (colorPoints[i + 1].color.b - colorPoints[i].color.b));
            color.a = static_cast<sf::Uint8>(colorPoints[i].color.a + 
                          factor * (colorPoints[i + 1].color.a - colorPoints[i].color.a));
            
            return color;
        }
    }
    
    // 默认返回白色
    return sf::Color::White;
}