/**
 * @file ParticleSystem.h
 * @brief 粒子系统类头文件
 * 
 * 该文件定义了ParticleSystem类，用于管理和模拟粒子系统。
 * 粒子系统负责粒子的创建、更新、碰撞检测以及应用全局力等功能。
 */

#pragma once
#include "CoreParticle.h"
#include <vector>
#include <memory>

/**
 * @class ParticleSystem
 * @brief 粒子系统管理类
 * 
 * 该类负责管理所有粒子的生命周期，包括粒子的添加、移除、更新和碰撞检测。
 * 同时提供场景大小、重力等全局参数的设置功能。
 */
class ParticleSystem {
private:
    /** 存储系统中所有粒子的智能指针容器 */
    std::vector<std::shared_ptr<CoreParticle>> particles;
    
    /** 是否启用空间分区优化，用于提高大量粒子时的碰撞检测性能 */
    bool useSpatialPartitioning;
    
    /** 粒子系统暂停状态标志，暂停时粒子不更新 */
    bool isPaused;
    
    /** 场景宽度，用于边界碰撞检测 */
    float sceneWidth;
    
    /** 场景高度，用于边界碰撞检测 */
    float sceneHeight;
    
    /** 重力加速度，应用于所有粒子的全局力 */
    float gravityAcceleration;
    
public:
    /**
     * @brief 默认构造函数
     * 
     * 初始化粒子系统，设置默认参数值。
     */
    ParticleSystem();
    
    /**
     * @brief 默认析构函数
     */
    ~ParticleSystem() = default;
    
    /**
     * @brief 向粒子系统中添加一个粒子
     * @param particle 要添加的粒子的智能指针
     */
    void addParticle(std::shared_ptr<CoreParticle> particle);
    
    /**
     * @brief 从粒子系统中移除指定索引的粒子
     * @param index 要移除的粒子的索引
     */
    void removeParticle(size_t index);
    
    /**
     * @brief 清除粒子系统中的所有粒子
     */
    void clearParticles();
    
    /**
     * @brief 更新粒子系统状态
     * @param deltaTime 距离上一帧的时间增量（秒）
     * 
     * 该方法会更新所有粒子的位置、速度，并处理碰撞检测。
     * 如果系统处于暂停状态，则不执行更新。
     */
    void update(float deltaTime);
    
    /**
     * @brief 应用全局力到所有粒子
     * 
     * 当前主要应用重力，但可以扩展其他全局力如风力、电磁力等。
     */
    void applyGlobalForces();
    
    /**
     * @brief 处理粒子间的碰撞检测和响应
     * 
     * 检测所有粒子之间的碰撞，并根据物理规则计算碰撞后的速度变化。
     * 如果启用了空间分区，将使用优化算法提高性能。
     */
    void resolveCollisions();
    
    /**
     * @brief 获取粒子系统中的所有粒子
     * @return 粒子容器的常量引用
     */
    const std::vector<std::shared_ptr<CoreParticle>>& getParticles() const { return particles; }
    
    /**
     * @brief 获取粒子系统中的粒子数量
     * @return 粒子数量
     */
    size_t getParticleCount() const { return particles.size(); }
    
    /**
     * @brief 设置是否启用空间分区优化
     * @param enable true启用，false禁用
     * 
     * 空间分区可以显著提高大量粒子时的碰撞检测性能。
     */
    void setSpatialPartitioning(bool enable) { useSpatialPartitioning = enable; }
    
    /**
     * @brief 设置粒子系统的暂停状态
     * @param paused true暂停，false继续
     * 
     * 暂停状态下，粒子系统不会更新粒子的位置和速度。
     */
    void setPaused(bool paused) { isPaused = paused; }
    
    /**
     * @brief 获取粒子系统的暂停状态
     * @return true表示暂停，false表示运行中
     */
    bool isSystemPaused() const { return isPaused; }
    
    /**
     * @brief 设置场景大小
     * @param width 场景宽度
     * @param height 场景高度
     * 
     * 场景大小用于边界碰撞检测，粒子碰到边界会反弹。
     */
    void setSceneSize(float width, float height) { sceneWidth = width; sceneHeight = height; }
    
    /**
     * @brief 获取场景宽度
     * @return 场景宽度
     */
    float getSceneWidth() const { return sceneWidth; }
    
    /**
     * @brief 获取场景高度
     * @return 场景高度
     */
    float getSceneHeight() const { return sceneHeight; }
    
    /**
     * @brief 设置重力加速度
     * @param gravity 重力加速度值（正数表示向下）
     * 
     * 重力加速度会应用到所有粒子的垂直速度分量上。
     */
    void setGravityAcceleration(float gravity) { gravityAcceleration = gravity; }
    
    /**
     * @brief 获取当前重力加速度
     * @return 重力加速度值
     */
    float getGravityAcceleration() const { return gravityAcceleration; }
};/**
 * @file ParticleSystem.cpp
 * @brief 粒子系统类实现文件
 * 
 * 该文件实现了ParticleSystem类的所有方法，包括粒子的添加、更新、碰撞检测
 * 以及全局力的应用等功能。支持空间分区优化以提高大量粒子时的性能。
 */


