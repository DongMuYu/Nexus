#include "CoreParticle.h"
#include <cmath>
#include <iostream>

/**
 * @brief 构造函数：初始化粒子的物理属性和交互参数
 * @param pos 粒子的初始位置（世界坐标）
 * @param mass 粒子质量，影响惯性力和重力作用。质量为0的粒子为静态粒子
 * @param radius 粒子半径，用于渲染显示和碰撞检测
 *
 * 初始化列表按声明顺序初始化成员变量，确保正确的初始化顺序
 */
CoreParticle::CoreParticle(Vector2 pos, float mass, float radius)
    : position(pos), mass(mass), radius(radius),
    velocity(0, 0), acceleration(0, 0), forceAccumulator(0, 0),
    // rotation(0), angularVelocity(0), angularAcceleration(0),
    // torqueAccumulator(0), 
    color(sf::Color::White),
    // attractionRange(150.0f), equilibriumDistance(30.0f),
    // attractionStrength(50.0f), repulsionStrength(100.0f),
    // maxBonds(4), currentBonds(0), 
    restitution(1.0f),
    separationFactor(1.05f), // 默认增加5%的分离冗余量
    isCollidable(true), isStatic(false)
{
    // 计算质量的倒数，优化后续的 F = ma 计算（避免重复除法）
    inverseMass = mass > 0 ? 1.0f / mass : 0;

    // 计算转动惯量：对于实心圆盘，I = (1/2) * m * r²
    // 这里使用0.4近似1/2，可以根据需要调整这个系数
    // momentOfInertia = 0.4f * mass * radius * radius;
}

/**
 * @brief 更新粒子状态的主函数，每帧调用一次
 * @param deltaTime 时间步长（秒），用于数值积分
 *
 * 更新顺序：
 * 1. 先处理连接约束（弹簧力等）
 * 2. 再进行物理状态积分
 * 这样确保约束力能够影响当前帧的运动
 */
void CoreParticle::update(float deltaTime) {
    // 更新所有连接的约束力（弹簧力、阻尼力等）
    // updateBonds();

    // 执行物理数值积分，更新位置、速度、旋转等状态
    integrate(deltaTime);
}

/**
 * @brief 数值积分器：根据累积的力和扭矩更新运动状态
 * @param deltaTime 时间步长（秒）
 *
 * 使用显式欧拉积分方法：
 * 线性运动：a = F/m → v = v + a·Δt → x = x + v·Δt
 * 旋转运动：α = τ/I → ω = ω + α·Δt → θ = θ + ω·Δt
 *
 * 注意：欧拉积分简单但精度有限，后续可升级为Verlet或Runge-Kutta
 */
void CoreParticle::integrate(float deltaTime) {
    // ========== 线性运动积分 ==========
    if (mass > 0) {
        // 计算加速度：a = F/m
        acceleration = forceAccumulator * inverseMass;

        // 更新速度：v = v₀ + a·Δt
        velocity += acceleration * deltaTime;

        // 更新位置：x = x₀ + v·Δt
        position += velocity * deltaTime;
    }

    // ========== 旋转运动积分 ==========
    // if (momentOfInertia > 0) {
    //     // 计算角加速度：α = τ/I
    //     angularAcceleration = torqueAccumulator / momentOfInertia;

    //     // 更新角速度：ω = ω₀ + α·Δt
    //     angularVelocity += angularAcceleration * deltaTime;

    //     // 更新旋转角度：θ = θ₀ + ω·Δt
    //     rotation += angularVelocity * deltaTime;

    //     // 保持角度在[0, 2π)范围内，避免数值溢出
    //     rotation = std::fmod(rotation, 2 * 3.1415926535f);
    //     if (rotation < 0) rotation += 2 * 3.1415926535f;
    // }

    // 清空本帧累积的力和扭矩，为下一帧做准备
    clearAccumulators();

    // ========== 应用阻尼力 ==========
    // 速度阻尼：模拟空气阻力等耗散力，避免系统能量无限增加
    velocity *= 0.9995f;

    // 角速度阻尼：模拟旋转阻力
    // angularVelocity *= 0.95f;
}

/**
 * @brief 施加力到粒子，力会累积到累加器中
 * @param force 要施加的力向量（牛顿或任意一致单位）
 *
 * 注意：力不会立即影响运动状态，而是在integrate()中统一处理
 * 这种设计允许在同一帧中多次施加力，最后统一计算合力效果
 */
void CoreParticle::applyForce(const Vector2& force) {
    forceAccumulator += force;
}

/**
 * @brief 施加扭矩到粒子，影响旋转运动
 * @param torque 扭矩大小（牛顿·米或任意一致单位）
 *
 * 正扭矩产生逆时针旋转，负扭矩产生顺时针旋转
 * 扭矩同样会累积，在integrate()中统一处理
 */
// void CoreParticle::applyTorque(float torque) {
//     torqueAccumulator += torque;
// }

/**
 * @brief 清空力和扭矩累加器
 *
 * 每帧开始时调用，确保从零开始累积新的力和扭矩
 * 这是物理引擎的标准做法，避免上一帧的力影响当前帧
 */
void CoreParticle::clearAccumulators() {
    forceAccumulator = Vector2(0, 0);
    // torqueAccumulator = 0;
}

/**
 * @brief 计算Lennard-Jones势产生的力大小
 * @param distance 两粒子间的距离
 * @return 力的大小（正值为排斥力，负值为吸引力）
 *
 * Lennard-Jones势公式：U(r) = 4ε[(σ/r)¹² - (σ/r)⁶]
 * 对应的力：F(r) = -dU/dr = 24ε[2(σ/r)¹³ - (σ/r)⁷]/r
 *
 * 其中：
 * - σ = equilibriumDistance (势能零点距离)
 * - ε = attractionStrength (势能深度)
 * - r = distance (当前距离)
 *
 * 特性：
 * - 当 r → 0⁺: 强排斥力（防止穿透）
 * - 当 r = σ: 力为零（平衡点）
 * - 当 r > σ: 吸引力（分子间范德华力）
 * - 当 r → ∞: 力趋近于零
 */
// float CoreParticle::calculateLennardJonesForce(float distance) const {
//     // 防止除零错误，设置最小距离阈值
//     if (distance < 0.1f) distance = 0.1f;

//     // 计算比率：σ/r
//     float ratio = equilibriumDistance / distance;

//     // 预计算 (σ/r)⁶ 和 (σ/r)¹²
//     float ratio6 = ratio * ratio * ratio * ratio * ratio * ratio;
//     float ratio12 = ratio6 * ratio6;

//     // 完整的Lennard-Jones力公式
//     return 24.0f * attractionStrength * (2.0f * ratio12 - ratio6) / distance;
// }

/**
 * @brief 计算并应用与另一个粒子的相互作用力
 * @param other 另一个粒子的共享指针
 *
 * 基于Lennard-Jones势函数计算相互作用力，并同时应用到两个粒子上
 * 遵循牛顿第三定律：作用力与反作用力大小相等、方向相反
 */
void CoreParticle::applyParticleInteraction(std::shared_ptr<CoreParticle> other) {
    // 计算两粒子间的相对位置向量
    // Vector2 delta = other->position - position;
    // float distance = delta.length();

    // 距离检查：超出作用范围或太近则忽略相互作用
    // if (distance > attractionRange || distance < 0.1f) {
    //     return;
    // }

    // 计算Lennard-Jones力的大小和方向
    // float forceMagnitude = calculateLennardJonesForce(distance);
    // Vector2 force = delta.normalized() * forceMagnitude;

    // 应用力到两个粒子（牛顿第三定律）
    // applyForce(force);           // 本粒子受到的作用力
    // other->applyForce(-force);   // 另一个粒子受到的反作用力

    // ========== 计算并应用扭矩 ==========
    // 扭矩产生于力的作用点不在质心上
    // 对于圆形粒子，使用相对位置的一半作为杠杆臂
    // Vector2 leverArm = delta * 0.5f;

    // 2D扭矩计算：τ = r × F = r_x·F_y - r_y·F_x
    // float torque = leverArm.cross(force);

    // 应用扭矩到两个粒子
    // applyTorque(torque);
    // other->applyTorque(-torque); // 反方向的扭矩
}

/**
 * @brief 检查是否可以形成新的连接
 * @return 如果当前连接数小于最大连接数，返回true
 *
 * 用于模拟化学价键概念，限制每个粒子的最大连接数
 */
// bool CoreParticle::canFormBond() const {
//     return currentBonds < maxBonds;
// }

/**
 * @brief 与另一个粒子形成连接
 * @param other 目标粒子
 * @param strength 连接强度（类似弹簧系数k）
 * @param length 连接的自然长度（平衡长度）
 * @param maxStretch 最大拉伸倍数（如1.5表示可拉伸50%）
 * @return 如果成功形成连接返回true，否则返回false
 *
 * 形成双向连接，两个粒子都会记录这个连接关系
 */
// bool CoreParticle::formBond(std::shared_ptr<CoreParticle> other, float strength,
//     float length, float maxStretch) {
//     // 检查两个粒子是否都可以形成新连接
//     if (!canFormBond() || !other->canFormBond()) {
//         return false;
//     }

//     // 创建新的连接结构
//     Bond newBond;
//     newBond.target = other;          // 弱引用指向目标粒子
//     newBond.bondStrength = strength; // 连接刚度
//     newBond.bondLength = length;     // 自然长度
//     newBond.maxStretch = maxStretch; // 最大拉伸限度
//     newBond.isBroken = false;        // 初始状态为未断裂

//     // 添加到连接列表并更新计数
//     bonds.push_back(newBond);
//     currentBonds++;

//     return true;
// }

/**
 * @brief 断开指定索引的连接
 * @param index 要断开的连接在bonds向量中的索引
 *
 * 从连接列表中移除指定连接，并更新连接计数
 * 注意：这不会自动断开对方粒子的对应连接
 */
// void CoreParticle::breakBond(int index) {
//     // 检查索引有效性
//     if (index >= 0 && index < bonds.size()) {
//         bonds.erase(bonds.begin() + index);
//         currentBonds--;
//     }
// }

/**
 * @brief 更新所有连接的状态，计算并应用约束力
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
// void CoreParticle::updateBonds() {
//     // 遍历所有连接
//     for (auto& bond : bonds) {
//         // 检查目标粒子是否仍然存在（弱引用转强引用）
//         if (auto target = bond.target.lock()) {
//             // 计算当前连接向量和长度
//             Vector2 delta = target->position - position;
//             float currentLength = delta.length();
//             float stretch = currentLength - bond.bondLength;

//             // 检查是否超过最大拉伸限度
//             if (std::abs(stretch) > bond.bondLength * bond.maxStretch) {
//                 bond.isBroken = true; // 标记为断裂
//                 continue;             // 跳过这个连接的处理
//             }

//             // 如果连接已断裂，跳过处理
//             if (bond.isBroken) continue;

//             // ========== 计算弹簧力 ==========
//             // 胡克定律：F_spring = -k · Δx
//             // 方向沿连接方向，大小与拉伸量成正比
//             Vector2 springForce = delta.normalized() * stretch * bond.bondStrength;

//             // 应用弹簧力到两个粒子（作用力与反作用力）
//             applyForce(springForce);
//             target->applyForce(-springForce);

//             // ========== 计算阻尼力 ==========
//             // 阻尼力与相对速度成正比，方向与相对速度相反
//             // 用于减少连接的振荡和弹性振动
//             Vector2 relativeVelocity = target->velocity - velocity;
//             Vector2 dampingForce = relativeVelocity * 0.1f * bond.bondStrength;

//             // 应用阻尼力到两个粒子
//             applyForce(dampingForce);
//             target->applyForce(-dampingForce);
//         }
//     }

//     // ========== 清理已断裂的连接 ==========
//     // 使用remove-erase惯用法移除所有已断裂的连接
//     bonds.erase(std::remove_if(bonds.begin(), bonds.end(),
//         [](const Bond& bond) { return bond.isBroken; }), bonds.end());
// }
/**
 * @brief 判断是否需要碰撞以及解决与另一个粒子的碰撞
 * @param other 另一个粒子的共享指针
 * 
 * 使用完全弹性碰撞解决碰撞，先进行位置调整，再根据质量、速度和碰撞点来分配速度
 * 基于动量守恒和能量守恒原理
 */
void CoreParticle::resolveCollision(std::shared_ptr<CoreParticle> other) {
    // 如果任一粒子不参与碰撞检测，则不进行碰撞检测
    if (UNLIKELY(!isCollidable || !other->isCollidable || this == other.get())) {
        return;
    }
    
    // 如果两个都是静态粒子，不需要检测碰撞
    if (UNLIKELY(isStatic && other->isStatic)) {
        return;
    }
    
    // 1. 缓存频繁访问的数据（减少指针解引用）
    const Vector2 other_pos = other->position;
    const Vector2 other_vel = other->velocity;
    const float other_rad = other->radius;
    const float other_mass = other->mass;
    
    // 2. 预计算增量向量和距离平方
    const Vector2 delta = other_pos - position;
    const float distSq = delta.x * delta.x + delta.y * delta.y; // 直接计算，避免方法调用
    const float minDist = radius + other_rad;
    const float minDistSq = minDist * minDist;

    // 快速检查：是否可能碰撞（距离平方比较更快）
    if (LIKELY(distSq > minDistSq + 0.001f)) {
        return;
    }
    
    // 2. 计算实际距离和法线（只在必要时开方）
    const float distance = std::sqrt(distSq);
    if (UNLIKELY(distance < 1e-6f)) {
        // 处理重叠情况：使用随机方向
        const Vector2 normal(1.0f, 0.0f); // 预定义常量可能更好
        handleOverlap(other, normal, minDist);
        return;
    }

    // 使用倒数优化法线计算
    const float invDistance = 1.0f / distance;
    const Vector2 normal = delta * invDistance;
    
    // 3. 相对速度计算和早期退出检查
    const Vector2 relativeVelocity = other->velocity - velocity;
    const float velAlongNormal = relativeVelocity.dot(normal);
    
    // 如果粒子正在分离，则不需要解决碰撞
    // 只有当相对速度沿法线方向大于0时，才需要解决碰撞
    // 相对速度为正，说明前一个粒子的速度大于后一个粒子的速度
    // 如果正在分离，尽早返回
    if (velAlongNormal > -1e-6f) {
        return;
    }
    
    // 4. 位置修正
    const float overlap = minDist - distance;
    if (overlap > 0.0f) {
        correctPositions(other, normal, overlap);
    }
    
    // 完全弹性碰撞的速度计算
    // 5. 预计算质量相关参数
    const float m1 = mass;
    const float m2 = other->mass;
    const float totalMass = m1 + m2;
    const float massDiff  = m1 - m2;
    const float invTotalMass = 1.0f / totalMass; // 除法变乘法

    // 6. 直接计算法向速度分量（避免重复dot计算）
    const float v1n = velocity.dot(normal);
    const float v2n = other->velocity.dot(normal);
    
    // 7. 使用优化后的弹性碰撞公式
    const float v1Final = (massDiff * v1n + 2.0f * m2 * v2n) * invTotalMass;
    const float v2Final = (massDiff * v2n + 2.0f * m1 * v1n) * invTotalMass;
    
    // 更新法向速度分量并加上切向分量
    velocity += normal * (v1Final - v1n);
    other->velocity += normal * (v2Final - v2n);
}


/**
 * @brief 检测与墙壁的碰撞并解决
 * @param width 场景宽度
 * @param height 场景高度
 * 
 * 处理粒子与场景边界的碰撞，包括位置修正和速度更新
 * 实现完全弹性碰撞
 */
void CoreParticle::checkAndResolveWallCollision(float width, float height) {
    // 如果粒子不参与碰撞检测，则不进行碰撞检测
    if (!isCollidable) {
        return;
    }
    
    // 完全弹性碰撞，恢复系数为1.0
    // 左墙碰撞检测
    if (position.x - radius < 0) {
        position.x = radius; // 位置修正，使用调整后的半径
        if (!isStatic) {
            velocity.x = -velocity.x * restitution; // 速度反转并应用恢复系数
        }
    }
    
    // 右墙碰撞检测
    if (position.x + radius > width) {
        position.x = width - radius; // 位置修正，使用调整后的半径
        if (!isStatic) {
            velocity.x = -velocity.x * restitution; // 速度反转并应用恢复系数
        }
    }
    
    // 上墙碰撞检测
    if (position.y - radius < 0) {
        position.y = radius; // 位置修正，使用调整后的半径
        if (!isStatic) {
            velocity.y = -velocity.y * restitution; // 速度反转并应用恢复系数
        }
    }
    
    // 下墙碰撞检测
    if (position.y + radius > height) {
        position.y = height - radius; // 位置修正，使用调整后的半径
        if (!isStatic) {
            velocity.y = -velocity.y * restitution; // 速度反转并应用恢复系数
        }
    }
}

