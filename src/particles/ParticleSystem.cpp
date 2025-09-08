
#include "ParticleSystem.h"
#include "../physics/SpatialHash.h"
#include <iostream>

/**
 * @brief 粒子系统构造函数
 * 
 * 初始化粒子系统的各项参数，包括启用空间分区、设置默认场景大小和重力加速度。
 */
ParticleSystem::ParticleSystem() : useSpatialPartitioning(true), isPaused(false), sceneWidth(720.0f), sceneHeight(720.0f), gravityAcceleration(9.8f) {
    std::cout << "[DEBUG] ParticleSystem initialized" << std::endl;
}

/**
 * @brief 向粒子系统中添加一个粒子
 * @param particle 要添加的粒子的智能指针
 * 
 * 将粒子添加到粒子容器中，使其成为系统的一部分。
 */
void ParticleSystem::addParticle(std::shared_ptr<CoreParticle> particle) {
    particles.push_back(particle);
}

/**
 * @brief 更新粒子系统状态
 * @param deltaTime 距离上一帧的时间增量（秒）
 * 
 * 该方法是粒子系统的核心更新逻辑，按以下顺序执行：
 * 1. 检查系统是否暂停，暂停则直接返回
 * 2. 清除所有粒子的力累加器
 * 3. 应用全局力（如重力）
 * 4. 处理粒子间的相互作用
 *    - 如果启用空间分区，使用空间哈希优化
 *    - 否则使用暴力检测所有粒子对
 * 5. 更新所有粒子的位置和速度
 * 6. 处理粒子间和粒子与墙壁的碰撞
 */
void ParticleSystem::update(float deltaTime) {
    // 如果粒子系统被暂停，则不进行任何更新
    if (isPaused) {
        static bool debugPrinted = false;
        if (!debugPrinted) {
            std::cout << "[DEBUG] ParticleSystem update skipped due to pause" << std::endl;
            debugPrinted = true;
        }
        return;
    }
    
    // 重置调试标志
    static bool debugPrintedResume = false;
    if (!debugPrintedResume) {
        std::cout << "[DEBUG] ParticleSystem update resumed" << std::endl;
        debugPrintedResume = true;
    }
    
    // 清除所有力的累加器，为新一轮的力累加做准备
    for (auto& particle : particles) {
        particle->clearAccumulators();
    }
    
    // 应用全局力（重力等）
    applyGlobalForces();
    
    // 粒子间相互作用
    if (useSpatialPartitioning) {
        // 使用空间哈希优化，将粒子分配到空间网格中，只检测相邻网格中的粒子
        SpatialHash spatialHash(50.0f); // 单元格大小
        for (auto& particle : particles) {
            spatialHash.addParticle(particle);
        }
        
        spatialHash.processInteractions();
    } else {
        // 暴力检测，遍历所有粒子对，计算复杂度为O(n²)
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                particles[i]->applyParticleInteraction(particles[j]);
            }
        }
    }
    
    // 更新所有粒子的位置和速度
    for (auto& particle : particles) {
        particle->update(deltaTime);
    }
    
    // 处理碰撞
    resolveCollisions();
}

/**
 * @brief 应用全局力到所有粒子
 * 
 * 当前主要应用重力，但可以扩展其他全局力如风力、电磁力等。
 * 重力根据粒子的质量计算，质量越大，受到的重力越大。
 */
void ParticleSystem::applyGlobalForces() {
    for (auto& particle : particles) {
        // 应用重力，方向向下（Y轴正方向），大小为重力加速度乘以粒子质量
        particle->applyForce(Vector2(0, gravityAcceleration * particle->getMass()));
    }
}

/**
 * @brief 处理粒子间的碰撞检测和响应
 * 
 * 该方法分为两部分：
 * 1. 粒子间碰撞检测与解决
 *    - 如果启用空间分区，使用空间哈希优化碰撞检测
 *    - 否则使用暴力检测所有粒子对
 * 2. 墙壁碰撞检测与解决
 *    - 检测每个粒子是否与场景边界碰撞，并进行响应
 */
void ParticleSystem::resolveCollisions() {
    // ========== 粒子间碰撞检测与解决 ==========
    if (useSpatialPartitioning) {
        // 使用空间哈希优化碰撞检测，将粒子分配到空间网格中
        SpatialHash spatialHash(50.0f); // 单元格大小
        for (auto& particle : particles) {
            spatialHash.addParticle(particle);
        }
        
        // 处理空间哈希中的碰撞，只检测相邻网格中的粒子
        spatialHash.processCollisions();
    } else {
        // 暴力检测所有粒子对，计算复杂度为O(n²)
        for (size_t i = 0; i < particles.size(); i++) {
            for (size_t j = i + 1; j < particles.size(); j++) {
                // 检测碰撞
                particles[i]->resolveCollision(particles[j]);
            }
        }
    }
    
    // ========== 墙壁碰撞检测与解决 ==========
    for (auto& particle : particles) {
        // 使用粒子自带的墙壁碰撞检测方法，检测粒子是否与场景边界碰撞
        // 如果碰撞，则根据物理规则反弹粒子
        particle->checkAndResolveWallCollision(sceneWidth, sceneHeight);
    }
}