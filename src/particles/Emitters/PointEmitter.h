// src/particles/Emitters/PointEmitter.h
#pragma once
#include "BaseEmitter.h"

/**
 * @brief 点发射器 - 根据鼠标拖拽向量发射粒子
 * 
 * 该类实现了根据鼠标点击拖拽形成的向量来发射具有方向和速度的粒子。
 */
class PointEmitter : public BaseEmitter {
private:
    float particleMass;     // 粒子质量
    float particleRadius;   // 粒子半径
    sf::Color particleColor; // 粒子颜色
    float velocityScaleFactor; // 速度比例因子，用于调整拖动向量与最终速度的比例

public:
    PointEmitter(Vector2 pos = Vector2(0, 0),
        Vector2 dir = Vector2(0, -1),
        float speed = 100.0f,
        float mass = 1.0f,
        float radius = 15.0f,
        sf::Color color = sf::Color::White);

    // 重写基类方法
    void update(float deltaTime, std::vector<std::shared_ptr<CoreParticle>>& particles) override;
    std::shared_ptr<CoreParticle> emitParticle() override;

    // 设置粒子属性
    void setParticleProperties(float mass, float radius, sf::Color color);
    
    // 设置速度比例因子
    void setVelocityScaleFactor(float factor) { velocityScaleFactor = factor; }
    // 获取速度比例因子
    float getVelocityScaleFactor() const { return velocityScaleFactor; }
};