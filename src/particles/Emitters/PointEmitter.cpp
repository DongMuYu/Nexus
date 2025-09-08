// src/particles/Emitters/PointEmitter.cpp
#include "PointEmitter.h"

/**
 * @brief 构造函数：初始化点发射器的属性
 * @param pos 发射器的位置（世界坐标）
 * @param dir 发射方向向量（将被归一化）
 * @param speed 发射速度（像素/秒）
 * @param mass 粒子质量（影响物理行为）
 * @param radius 粒子半径（用于渲染和碰撞检测）
 * @param color 粒子颜色（SFML颜色）
 * 
 * 点发射器是一种特殊的发射器，它只在被激活时发射一个粒子，
 * 而不是持续发射。这使其非常适合用于鼠标拖拽发射粒子的场景。
 */
PointEmitter::PointEmitter(Vector2 pos, Vector2 dir, float speed, float mass, float radius, sf::Color color)
    : BaseEmitter(pos, dir, speed, 1.0f), // 发射速率设为1.0f，因为我们只需要根据鼠标拖拽发射一次
    particleMass(mass), particleRadius(radius), particleColor(color), velocityScaleFactor(2.0f) {
    isActive = false; // 初始状态为非激活，等待鼠标拖拽激活
}

/**
 * @brief 更新发射器状态
 * @param deltaTime 时间步长（秒）
 * @param particles 粒子系统的粒子容器引用
 * 
 * 点发射器的更新逻辑很简单：只有在激活状态下才发射一个粒子，
 * 然后立即将自己设为非激活状态。这种设计确保每次鼠标拖拽只发射一个粒子。
 */
void PointEmitter::update(float deltaTime, std::vector<std::shared_ptr<CoreParticle>>& particles) {
    if (!isActive) return;

    // 点发射器只需要在激活时发射一个粒子
    particles.push_back(emitParticle());
    isActive = false; // 发射后立即停用，确保每次只发射一个粒子
}

/**
 * @brief 创建并返回一个新的粒子
 * @return 新创建的粒子共享指针
 * 
 * 此方法负责创建具有特定属性的粒子，包括位置、质量、半径和颜色。
 * 特别重要的是，它根据鼠标拖拽向量设置粒子的初始速度。
 * 注意：不能直接访问粒子的velocity成员（它是protected的），
 * 而是使用applyForce方法来设置粒子的初始速度。
 */
std::shared_ptr<CoreParticle> PointEmitter::emitParticle() {
    // 创建新粒子，传入位置、质量和半径
    auto particle = std::make_shared<CoreParticle>(position, particleMass, particleRadius);

    // 设置粒子颜色
    particle->setColor(particleColor);

    // 直接设置粒子的初始速度，速度与拖动形成的向量成比例
    // 使用速度比例因子调整最终速度大小
    Vector2 initialVelocity = emissionDirection * emissionSpeed * velocityScaleFactor;
    particle->setVelocity(initialVelocity);

    // 设置粒子交互属性
    // particle->setAttractionRange(80.0f);      // 粒子相互作用范围
    // particle->setEquilibriumDistance(25.0f); // 粒子间平衡距离
    // particle->setMaxBonds(4);                // 每个粒子最多形成4个连接

    return particle;
}

/**
 * @brief 设置粒子属性
 * @param mass 粒子质量
 * @param radius 粒子半径
 * @param color 粒子颜色
 * 
 * 此方法允许动态调整发射器创建的粒子的基本属性，
 * 使发射器能够适应不同的游戏场景和视觉效果需求。
 */
void PointEmitter::setParticleProperties(float mass, float radius, sf::Color color) {
    particleMass = mass;
    particleRadius = radius;
    particleColor = color;
}