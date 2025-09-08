// src/physics/SpatialHash.h
#pragma once
#include "../particles/CoreParticle.h"
#include <unordered_map>
#include <vector>
#include <memory>

/**
 * @brief 空间哈希类，用于优化粒子间相互作用和碰撞检测
 * 
 * 将空间划分为网格，每个粒子根据其位置分配到相应的网格单元中
 * 只检查同一单元或相邻单元中的粒子对，减少不必要的计算
 */
class SpatialHash {
private:
    // 网格单元大小（像素）
    float cellSize;
    
    // 哈希表，键为网格坐标，值为该网格中的粒子列表
    std::unordered_map<int, std::vector<std::shared_ptr<CoreParticle>>> grid;
    
    /**
     * @brief 将世界坐标转换为网格坐标
     * @param x 世界坐标x
     * @param y 世界坐标y
     * @return 网格坐标的哈希值
     */
    int hashPosition(float x, float y) const;
    
    /**
     * @brief 获取粒子周围的网格单元
     * @param particle 粒子
     * @return 周围网格单元的哈希值列表
     */
    std::vector<int> getNearbyCells(const std::shared_ptr<CoreParticle>& particle) const;

public:
    /**
     * @brief 构造函数
     * @param cellSize 网格单元大小（像素）
     */
    SpatialHash(float cellSize = 50.0f);
    
    /**
     * @brief 清空哈希表
     */
    void clear();
    
    /**
     * @brief 添加粒子到哈希表
     * @param particle 要添加的粒子
     */
    void addParticle(const std::shared_ptr<CoreParticle>& particle);
    
    /**
     * @brief 处理粒子间相互作用
     * 
     * 遍历所有粒子，只检查同一单元或相邻单元中的粒子对
     * 应用Lennard-Jones势函数计算相互作用力
     */
    void processInteractions();
    
    /**
     * @brief 处理粒子间碰撞
     * 
     * 遍历所有粒子，只检查同一单元或相邻单元中的粒子对
     * 检测并解决碰撞
     */
    void processCollisions();
};