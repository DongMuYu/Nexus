// src/physics/SpatialHash.cpp
#include "SpatialHash.h"
#include <cmath>

/**
 * @brief 构造函数
 * @param cellSize 网格单元大小（像素）
 */
SpatialHash::SpatialHash(float cellSize) : cellSize(cellSize) {}

/**
 * @brief 清空哈希表
 */
void SpatialHash::clear() {
    grid.clear();
}

/**
 * @brief 将世界坐标转换为网格坐标
 * @param x 世界坐标x
 * @param y 世界坐标y
 * @return 网格坐标的哈希值
 * 
 * 使用简单的整数除法将世界坐标映射到网格坐标
 * 然后使用一个简单的哈希函数将二维网格坐标映射到一维整数
 */
int SpatialHash::hashPosition(float x, float y) const {
    // 将世界坐标转换为网格坐标
    int cellX = static_cast<int>(std::floor(x / cellSize));
    int cellY = static_cast<int>(std::floor(y / cellSize));
    
    // 使用一个简单的哈希函数将二维网格坐标映射到一维整数
    // 这里使用了一个常见的质数乘法哈希函数
    return cellX * 73856093 ^ cellY * 19349663;
}

/**
 * @brief 获取粒子周围的网格单元
 * @param particle 粒子
 * @return 周围网格单元的哈希值列表
 * 
 * 获取粒子所在网格及其周围8个相邻网格的哈希值
 * 这样可以确保不会漏掉任何可能发生相互作用的粒子对
 */
std::vector<int> SpatialHash::getNearbyCells(const std::shared_ptr<CoreParticle>& particle) const {
    std::vector<int> nearbyCells;
    
    // 获取粒子位置
    Vector2 position = particle->getPosition();
    
    // 将世界坐标转换为网格坐标
    int cellX = static_cast<int>(std::floor(position.x / cellSize));
    int cellY = static_cast<int>(std::floor(position.y / cellSize));
    
    // 获取周围3x3网格的哈希值
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nearbyCellX = cellX + dx;
            int nearbyCellY = cellY + dy;
            
            // 计算哈希值并添加到列表
            int hash = nearbyCellX * 73856093 ^ nearbyCellY * 19349663;
            nearbyCells.push_back(hash);
        }
    }
    
    return nearbyCells;
}

/**
 * @brief 添加粒子到哈希表
 * @param particle 要添加的粒子
 */
void SpatialHash::addParticle(const std::shared_ptr<CoreParticle>& particle) {
    // 获取粒子位置
    Vector2 position = particle->getPosition();
    
    // 计算哈希值
    int hash = hashPosition(position.x, position.y);
    
    // 将粒子添加到对应的网格单元
    grid[hash].push_back(particle);
}

/**
 * @brief 处理粒子间相互作用
 * 
 * 遍历所有粒子，只检查同一单元或相邻单元中的粒子对
 * 应用Lennard-Jones势函数计算相互作用力
 */
void SpatialHash::processInteractions() {
    // 遍历所有网格单元
    for (auto& cell : grid) {
        // 获取当前网格单元中的所有粒子
        std::vector<std::shared_ptr<CoreParticle>>& cellParticles = cell.second;
        
        // 遍历当前网格单元中的所有粒子对
        for (size_t i = 0; i < cellParticles.size(); i++) {
            // 检查当前粒子与同一网格单元中的其他粒子
            for (size_t j = i + 1; j < cellParticles.size(); j++) {
                // 应用粒子间相互作用
                cellParticles[i]->applyParticleInteraction(cellParticles[j]);
            }
        }
    }
}

/**
 * @brief 处理粒子间碰撞
 * 
 * 遍历所有粒子，只检查同一单元或相邻单元中的粒子对
 * 检测并解决碰撞
 */
void SpatialHash::processCollisions() {
    // 遍历所有网格单元
    for (auto& cell : grid) {
        // 获取当前网格单元中的所有粒子
        std::vector<std::shared_ptr<CoreParticle>>& cellParticles = cell.second;
        
        // 遍历当前网格单元中的所有粒子对
        for (size_t i = 0; i < cellParticles.size(); i++) {
            // 检查当前粒子与同一网格单元中的其他粒子
            for (size_t j = i + 1; j < cellParticles.size(); j++) {
                // 检测碰撞
                cellParticles[i]->resolveCollision(cellParticles[j]);
            }
        }
    }
}