#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "../particles/ParticleSystem.h"
#include "../particles/CoreParticle.h"

class Renderer {
public:
    // 渲染模式枚举
    enum class RenderMode {
        Default,
        Velocity,
        Force,
        Connections
    };

    // 构造函数
    Renderer(sf::RenderWindow& window, ParticleSystem& particleSystem);
    
    // 析构函数
    ~Renderer() = default;
    
    // 主要渲染方法
    void render();
    
    // 设置方法
    void setShowParticleConnections(bool show);
    void setShowParticleTrails(bool show);
    void setShowVelocityVectors(bool show);
    void setShowForceVectors(bool show);
    void setRenderMode(RenderMode mode);
    void setBackgroundColor(sf::Color color);
    void setConnectionColor(sf::Color color);
    
    // 获取方法
    bool getShowParticleConnections() const;
    bool getShowParticleTrails() const;
    bool getShowVelocityVectors() const;
    bool getShowForceVectors() const;
    RenderMode getRenderMode() const;
    sf::Color getBackgroundColor() const;
    sf::Color getConnectionColor() const;
    
    // 暂停状态方法
    void setPaused(bool paused);
    bool isRendererPaused() const;

private:
    // 私有方法
    void initializeRenderSettings();
    void renderDefault();
    void renderByVelocity();
    void renderByForce();
    void renderConnections();
    void renderParticle(std::shared_ptr<CoreParticle> particle, sf::Color color = sf::Color::White);
    void renderParticleConnections();
    void renderVelocityVectors();
    void renderForceVectors();
    void drawArrowHead(Vector2 start, Vector2 end, sf::Color color);
    
    // 成员变量
    sf::RenderWindow& window;
    ParticleSystem& particleSystem;
    
    // 渲染选项
    bool showParticleConnections;
    bool showParticleTrails;
    bool showVelocityVectors;
    bool showForceVectors;
    RenderMode renderMode;
    
    // 暂停状态
    bool isPaused;
    
    // 颜色设置
    sf::Color backgroundColor;
    sf::Color connectionColor;
    sf::Color velocityVectorColor;
    sf::Color forceVectorColor;
    
    // 颜色渐变类
    class ColorGradient {
    public:
        struct ColorPoint {
            float position;
            sf::Color color;
        };
        
        void addColorPoint(float position, sf::Color color);
        sf::Color getColor(float position) const;
        
    private:
        std::vector<ColorPoint> colorPoints;
    };
    
    ColorGradient particleGradient;
};