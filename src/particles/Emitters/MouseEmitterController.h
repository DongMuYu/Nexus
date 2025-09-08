// src/particles/Emitters/MouseEmitterController.h
#pragma once
#include "PointEmitter.h"
#include <SFML/Graphics.hpp>

/**
 * @brief 鼠标粒子发射器控制器
 * 该类负责管理鼠标交互相关的粒子发射器，支持根据鼠标拖拽向量发射粒子
 */
class MouseEmitterController
{
private:
    std::vector<std::unique_ptr<BaseEmitter>> emitters; ///< 所有发射器的集合
    Vector2 dragStartPosition;                          ///< 拖拽起始位置
    bool isDragging;                                    ///< 是否正在拖拽
    sf::RenderWindow &window;                           ///< 渲染窗口引用
    bool isPaused;                                      ///< 是否暂停

public:
    /**
     * @brief 构造函数
     * @param win 渲染窗口引用
     */
    MouseEmitterController(sf::RenderWindow &win);

    /**
     * @brief 处理SFML事件
     * @param event SFML事件对象
     * @param particles 粒子容器引用
     */
    void handleEvent(const sf::Event &event, std::vector<std::shared_ptr<CoreParticle>> &particles);

    /**
     * @brief 更新发射器状态
     * @param deltaTime 帧时间差
     * @param particles 粒子容器引用
     */
    void update(float deltaTime, std::vector<std::shared_ptr<CoreParticle>> &particles);

    /**
     * @brief 渲染发射器相关图形
     * @param window 渲染窗口
     */
    void render(sf::RenderWindow &window);

    /**
     * @brief 设置暂停状态
     * @param paused 是否暂停
     */
    void setPaused(bool paused);

    /**
     * @brief 获取暂停状态
     * @return 是否暂停
     */
    bool isControllerPaused() const;

private:
    /**
     * @brief 创建点发射器
     * @param position 发射位置
     * @param direction 发射方向
     * @param speed 发射速度
     */
    void createPointEmitter(Vector2 position, Vector2 direction, float speed);

    /**
     * @brief 渲染拖拽指示器
     * @param window 渲染窗口
     */
    void renderDragIndicator(sf::RenderWindow &window) const;
};