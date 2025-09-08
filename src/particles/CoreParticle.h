// CoreParticle.h - 核心粒子类头文件
// 编码格式：UTF-8 with BOM（防止中文注释乱码）
#pragma once

// ==================== 包含必要的头文件 ====================
#include <SFML/Graphics.hpp>       // SFML图形库，用于渲染和颜色处理
#include "../utils/Math/Vector2.h" // 自定义2D向量数学库
#include <vector>                  // 标准向量容器，用于动态数组管理
#include <memory>                  // 智能指针库，用于自动内存管理

// ==================== 使用标准命名空间别名 ====================
using std::shared_ptr; // 使用共享智能指针（自动引用计数）
using std::vector;     // 使用标准向量容器
using std::weak_ptr;   // 使用弱智能指针（避免循环引用）

/**
 * @brief 核心粒子类 - 2D物理模拟系统的基础单元
 *
 * 这个类实现了基本的物理粒子特性，包括：
 * - 牛顿力学运动（线性运动和旋转运动）
 * - Lennard-Jones势相互作用（吸引和排斥）
 * - 可配置的连接系统（模拟化学键或刚性连接）
 * - 可视化属性管理
 *
 * 设计为可扩展的基类，可以通过继承实现特殊粒子行为
 * 如流体、固体、气体等不同物质状态的粒子
 */
class CoreParticle
{
protected:
    // ==================== 基本物理属性 ====================
    Vector2 position;         ///< 当前位置（世界坐标系中的像素坐标）
    Vector2 velocity;         ///< 当前速度向量（像素/秒）
    Vector2 acceleration;     ///< 当前加速度向量（像素/秒²）
    Vector2 forceAccumulator; ///< 力累加器，用于累积一帧内所有作用力

    float mass;        ///< 粒子质量（任意单位，建议保持一致性），质量为0的粒子为静态粒子
    float inverseMass; ///< 质量的倒数，优化物理计算（避免重复除法运算）
    float radius;      ///< 粒子半径（像素），用于渲染显示和碰撞检测

    // ==================== 旋转运动属性 ====================
    // float rotation;            ///< 当前旋转角度（弧度制），0表示向右，π/2表示向下
    // float angularVelocity;     ///< 角速度（弧度/秒），正值为逆时针旋转
    // float angularAcceleration; ///< 角加速度（弧度/秒²）
    // float torqueAccumulator;   ///< 扭矩累加器，累积旋转力矩
    // float momentOfInertia;     ///< 转动惯量，决定旋转难易程度 I = k * m * r²

    // ==================== 视觉渲染属性 ====================
    sf::Color color; ///< 粒子颜色，用于SFML渲染显示

    // ==================== 粒子交互属性 ====================
    // float attractionRange;     ///< 相互作用范围半径（像素），超出此距离无相互作用
    // float equilibriumDistance; ///< 平衡距离（像素），Lennard-Jones势能最低点
    // float attractionStrength;  ///< 吸引强度系数，控制远距离吸引力大小
    // float repulsionStrength;   ///< 排斥强度系数，控制近距离排斥力大小

    // ==================== 连接系统（模拟化学键/刚性连接） ====================
    /**
     * @brief 连接结构体，描述两个粒子间的约束关系
     * 用于模拟化学键、弹簧连接或刚性约束
     */
    // struct Bond
    // {
    //     weak_ptr<CoreParticle> target; ///< 弱引用指向连接的目标粒子（避免循环引用导致内存泄漏）
    //     float bondStrength;            ///< 连接强度（类似弹簧系数k），值越大连接越刚性
    //     float bondLength;              ///< 自然长度（像素），连接松弛时的长度
    //     float maxStretch;              ///< 最大拉伸倍数，超过此值连接断裂（如1.5表示可拉伸50%）
    //     bool isBroken;                 ///< 连接是否已断裂标志
    // };

    // vector<Bond> bonds; ///< 当前粒子的所有连接列表
    // int maxBonds;       ///< 最大连接数限制（模拟化学价键概念）
    // int currentBonds;   ///< 当前连接数

    // ==================== 碰撞检测属性 ====================
    float restitution;      ///< 恢复系数（0-1之间），控制碰撞后的能量损失。1为完全弹性碰撞，0为完全非弹性碰撞
    float separationFactor; ///< 碰撞分离冗余因子（>1），用于增加碰撞分离的距离，避免粒子粘连
    bool isCollidable;      ///< 是否参与碰撞检测的标志
    bool isStatic;          ///< 是否为静态物体（不移动，但可以与其他物体碰撞）

public:
    // ==================== 构造与析构 ====================
    /**
     * @brief 构造函数：初始化粒子的物理属性和交互参数
     * @param pos 粒子的初始位置（世界坐标）
     * @param mass 粒子质量，影响惯性力和重力作用。质量为0的粒子为静态粒子
     * @param radius 粒子半径，用于渲染显示和碰撞检测
     *
     * 初始化列表按声明顺序初始化成员变量，确保正确的初始化顺序
     */
    CoreParticle(Vector2 pos, float mass = 1.0f, float radius = 8.0f);

    /// @brief 虚析构函数，确保派生类正确释放资源
    virtual ~CoreParticle() = default;

    // ==================== 物理更新接口 ====================
    /**
     * @brief 更新粒子状态（主更新函数）
     * @param deltaTime 时间步长（秒）
     *
     * 执行顺序：
     * 1. 更新连接状态
     * 2. 数值积分更新物理状态
     * 这样确保约束力能够影响当前帧的运动
     */
    virtual void update(float deltaTime);

    /**
     * @brief 数值积分器：根据力和扭矩更新运动状态
     * @param deltaTime 时间步长（秒）
     *
     * 实现欧拉积分：
     * - 线性运动：F = ma → v = v + a·Δt → x = x + v·Δt
     * - 旋转运动：τ = Iα → ω = ω + α·Δt → θ = θ + ω·Δt
     */
    void integrate(float deltaTime);

    // ==================== 力学作用接口 ====================
    /**
     * @brief 施加力到粒子（线性运动）
     * @param force 力向量（任意单位，但需保持一致性）
     *
     * 注意：力会累积到forceAccumulator，在integrate()中统一处理
     * 这种设计允许在同一帧中多次施加力，最后统一计算合力效果
     */
    void applyForce(const Vector2 &force);

    /**
     * @brief 施加扭矩到粒子（旋转运动）
     * @param torque 扭矩大小（顺时针为负，逆时针为正）
     *
     * 扭矩同样会累积，在integrate()中统一处理
     */
    // void applyTorque(float torque);

    /// @brief 清空力和扭矩累加器，每帧开始时调用
    void clearAccumulators();

    // ==================== 粒子间相互作用 ====================
    /**
     * @brief 计算并应用与另一个粒子的相互作用力
     * @param other 另一个粒子的共享指针
     *
     * 基于Lennard-Jones势函数：
     * - 近距离：强排斥（防止粒子重叠）
     * - 平衡点：力为零（稳定状态）
     * - 远距离：弱吸引（模拟分子间作用力）
     *
     * 遵循牛顿第三定律：作用力与反作用力大小相等、方向相反
     */
    virtual void applyParticleInteraction(shared_ptr<CoreParticle> other);

    /**
     * @brief 计算Lennard-Jones力大小
     * @param distance 两粒子间距离
     * @return 力的大小（正值为排斥，负值为吸引）
     *
     * 公式：F = 24ε * (2(σ/r)¹³ - (σ/r)⁷) / r
     * 其中σ=平衡距离，ε=能量深度
     */
    // float calculateLennardJonesForce(float distance) const;

    // ==================== 连接管理系统 ====================
    /// @return 是否可以形成新连接（未达到最大连接数）
    // bool canFormBond() const;

    /**
     * @brief 与另一个粒子形成连接
     * @param other 目标粒子
     * @param strength 连接强度（类似弹簧系数）
     * @param length 自然长度
     * @param maxStretch 最大拉伸倍数
     * @return 是否成功形成连接
     *
     * 形成双向连接，两个粒子都会记录这个连接关系
     */
    // bool formBond(shared_ptr<CoreParticle> other, float strength = 100.0f,
    //               float length = 20.0f, float maxStretch = 1.5f);

    // /// @brief 断开指定索引的连接
    // void breakBond(int index);

    /**
     * @brief 更新所有连接状态
     *
     * 对每个连接执行：
     * 1. 检查连接是否有效（目标粒子是否存在）
     * 2. 计算当前距离和拉伸量
     * 3. 检查是否超过最大拉伸，标记断裂
     * 4. 计算并应用弹簧力和阻尼力
     * 5. 清理已断裂的连接
     *
     * 使用胡克定律：F = -k·Δx
     * 和速度阻尼：F_damping = -c·v
     */
    void updateBonds();

    // ==================== 属性访问器 ====================
    Vector2 getPosition() const { return position; }
    Vector2 getVelocity() const { return velocity; }
    float getMass() const { return mass; }
    float getRadius() const { return radius; }
    float getRestitution() const { return restitution; }
    float getSeparationFactor() const { return separationFactor; }
    bool isCollidableParticle() const { return isCollidable; }
    bool isStaticParticle() const { return isStatic; }
    // int getBondCount() const { return currentBonds; }

    // ==================== 属性设置器 ====================
    void setPosition(const Vector2 &pos) { position = pos; }
    void setVelocity(const Vector2 &vel) { velocity = vel; }
    void setMass(float m)
    {
        mass = m;
        inverseMass = mass > 0 ? 1.0f / mass : 0;
    }
    void setRadius(float r) { radius = r; }
    void setRestitution(float r) { restitution = r; }
    void setSeparationFactor(float factor) { separationFactor = factor; }
    void setCollidable(bool collidable) { isCollidable = collidable; }
    void setStatic(bool staticParticle) { isStatic = staticParticle; }
    // void setAttractionRange(float range) { attractionRange = range; }
    // void setEquilibriumDistance(float distance) { equilibriumDistance = distance; }
    // void setMaxBonds(int max) { maxBonds = max; }

    // ==================== 碰撞检测方法 ====================
    /**
     * @brief 解决与另一个粒子的碰撞
     * @param other 另一个粒子的共享指针
     *
     * 使用冲量法解决碰撞，包括位置修正和速度更新
     */
    void resolveCollision(shared_ptr<CoreParticle> other);

    /**
     * @brief 检测与墙壁的碰撞并解决
     * @param width 场景宽度
     * @param height 场景高度
     *
     * 处理粒子与场景边界的碰撞
     */
    void checkAndResolveWallCollision(float width, float height);

    // ==================== 可视化相关 ====================
    virtual sf::Color getColor() const { return color; }
    virtual void setColor(sf::Color newColor) { color = newColor; }



private:
    // 分支预测宏
    #if defined(__GNUC__) || defined(__clang__)
    #define LIKELY(x) __builtin_expect(!!(x), 1)
    #define UNLIKELY(x) __builtin_expect(!!(x), 0)
    #else
    #define LIKELY(x) (x)
    #define UNLIKELY(x) (x)
    #endif

    // ==================== 辅助内联函数 ===================
    inline void CoreParticle::handleOverlap(std::shared_ptr<CoreParticle> other, 
                                        const Vector2& normal, float minDist) {
        // 处理严重重叠的情况
        const float separation = minDist * 0.5f;
        position -= normal * (separation * 0.5f);
        other->position += normal * (separation * 0.5f);
        
        // 给一个小随机速度避免卡住
        const float jitter = 0.01f;
        velocity += normal * jitter;
        other->velocity -= normal * jitter;
    }

    inline void CoreParticle::correctPositions(std::shared_ptr<CoreParticle> other,
                                            const Vector2& normal, float overlap) {
        const float totalRadius = radius + other->radius;
        const float ratio1 = radius / totalRadius;
        const float ratio2 = other->radius / totalRadius;
        
        position -= normal * (overlap * ratio1);
        other->position += normal * (overlap * ratio2);
    }
};
