// src/particles/Emitters/BaseEmitter.h
#pragma once
#include "../CoreParticle.h"
#include <memory>
#include <vector>

/**
 * @brief 粒子发射器基类
 * 
 * 该类定义了粒子发射器的基本属性和方法，所有具体的粒子发射器都应继承自该类。
 * 基类提供了发射器的通用接口，包括位置、方向、速度等基本属性，
 * 以及更新和发射粒子的纯虚函数，确保所有派生类实现这些核心功能。
 * 这种设计模式允许系统灵活地支持多种类型的粒子发射器，
 * 同时保持统一的接口和行为。
 */
class BaseEmitter {
protected:
    Vector2 position;                   ///< 发射器在世界空间中的位置坐标，以Vector2类型存储
    Vector2 emissionDirection;          ///< 粒子发射方向的单位向量，将被归一化处理
    float emissionSpeed;                ///< 粒子的初始发射速度，单位为像素/秒
    float emissionRate;                 ///< 粒子发射速率，表示每秒发射的粒子数量
    float timeSinceLastEmission;        ///< 自上次发射粒子以来经过的时间，用于控制发射间隔
    bool isActive;                      ///< 发射器是否处于激活状态，非激活状态的发射器不会更新或发射粒子

public:
    /**
     * @brief 构造函数
     * 
     * 初始化粒子发射器的基本属性
     * 
     * @param pos 发射器的初始位置，默认为坐标原点(0, 0)
     * @param dir 粒子的发射方向，默认为向上(0, -1)，将被自动归一化
     * @param speed 粒子的初始发射速度，默认为100.0像素/秒
     * @param rate 粒子的发射速率，默认为每秒1个粒子
     */
    BaseEmitter(Vector2 pos = Vector2(0, 0),
        Vector2 dir = Vector2(0, -1),
        float speed = 100.0f,
        float rate = 1.0f);
        
    /**
     * @brief 虚析构函数
     * 
     * 确保派生类对象通过基类指针删除时能正确调用派生类的析构函数
     */
    virtual ~BaseEmitter() = default;

    /**
     * @brief 更新发射器状态
     * 
     * 纯虚函数，由派生类实现具体逻辑。该方法在每一帧被调用，
     * 用于更新发射器的内部状态，并根据需要发射新粒子到粒子容器中。
     * 
     * @param deltaTime 距离上一帧的时间差，单位为秒，用于基于时间的计算
     * @param particles 粒子容器的引用，新发射的粒子将被添加到此容器中
     */
    virtual void update(float deltaTime, std::vector<std::shared_ptr<CoreParticle>>& particles) = 0;

    /**
     * @brief 发射单个粒子
     * 
     * 纯虚函数，由派生类实现具体逻辑。该方法创建并返回一个新的粒子对象，
     * 粒子的初始属性（位置、速度、颜色等）由发射器的当前状态决定。
     * 
     * @return 返回新创建的粒子的共享指针
     */
    virtual std::shared_ptr<CoreParticle> emitParticle() = 0;

    /**
     * @brief 设置发射器位置
     * 
     * @param pos 新的发射器位置坐标
     */
    void setPosition(Vector2 pos) { position = pos; }
    
    /**
     * @brief 设置发射方向
     * 
     * @param dir 新的发射方向向量，将被自动归一化
     */
    void setDirection(Vector2 dir) { emissionDirection = dir.normalized(); }
    
    /**
     * @brief 设置发射速度
     * 
     * @param speed 新的粒子发射速度，单位为像素/秒
     */
    void setSpeed(float speed) { emissionSpeed = speed; }
    
    /**
     * @brief 设置发射速率
     * 
     * @param rate 新的粒子发射速率，单位为粒子/秒
     */
    void setRate(float rate) { emissionRate = rate; }
    
    /**
     * @brief 设置发射器激活状态
     * 
     * @param active true表示激活发射器，false表示停用发射器
     */
    void setActive(bool active) { isActive = active; }

    /**
     * @brief 获取发射器位置
     * 
     * @return 返回发射器的当前位置坐标
     */
    Vector2 getPosition() const { return position; }
    
    /**
     * @brief 获取发射方向
     * 
     * @return 返回当前的发射方向单位向量
     */
    Vector2 getDirection() const { return emissionDirection; }
    
    /**
     * @brief 获取发射器激活状态
     * 
     * @return true表示发射器处于激活状态，false表示处于非激活状态
     */
    bool getIsActive() const { return isActive; }
};