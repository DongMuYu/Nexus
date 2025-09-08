// src/particles/Emitters/MouseEmitterController.cpp
#include "MouseEmitterController.h"
#include <iostream>

/**
 * @brief 构造函数
 * @param win 渲染窗口引用
 *
 * 初始化鼠标粒子发射器控制器，设置窗口引用并将拖拽状态初始化为false
 */
MouseEmitterController::MouseEmitterController(sf::RenderWindow &win)
    : window(win), isDragging(false), isPaused(false)
{
}

/**
 * @brief 处理SFML事件
 * @param event SFML事件对象
 * @param particles 粒子容器引用
 *
 * 处理鼠标按下和释放事件，实现拖拽发射粒子的功能：
 * 1. 鼠标左键按下时记录拖拽起始位置并设置拖拽状态
 * 2. 鼠标左键释放时计算拖拽向量，确定发射方向和速度，并创建点发射器
 */
void MouseEmitterController::handleEvent(const sf::Event &event, std::vector<std::shared_ptr<CoreParticle>> &particles)
{
    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (event.mouseButton.button == sf::Mouse::Left)
        {
            // 记录拖拽起始位置
            dragStartPosition = Vector2(event.mouseButton.x, event.mouseButton.y);
            isDragging = true;
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (event.mouseButton.button == sf::Mouse::Left && isDragging)
        {
            // 计算拖拽结束位置、方向和速度
            Vector2 endPos(event.mouseButton.x, event.mouseButton.y);
            Vector2 direction = (endPos - dragStartPosition).normalized();
            float speed = (endPos - dragStartPosition).length() * 0.5f;

            // 创建点发射器
            createPointEmitter(dragStartPosition, direction, speed);

            isDragging = false;
        }
    }
}

/**
 * @brief 更新所有发射器状态
 * @param deltaTime 帧时间差
 * @param particles 粒子容器引用
 *
 * 执行两项主要任务：
 * 1. 遍历所有发射器并调用其update方法，更新发射器状态并可能发射新粒子
 * 2. 清理不再活跃的发射器，移除已经完成发射任务的发射器以释放资源
 */
void MouseEmitterController::update(float deltaTime, std::vector<std::shared_ptr<CoreParticle>> &particles)
{
    // 如果控制器被暂停，则不进行任何更新
    if (isPaused)
    {
        static bool debugPrinted = false;
        if (!debugPrinted)
        {
            std::cout << "[DEBUG] MouseEmitterController update skipped due to pause" << std::endl;
            debugPrinted = true;
        }
        return;
    }

    // 重置调试标志
    static bool debugPrintedResume = false;
    if (!debugPrintedResume)
    {
        std::cout << "[DEBUG] MouseEmitterController update resumed" << std::endl;
        debugPrintedResume = true;
    }

    // 更新所有发射器
    for (auto &emitter : emitters)
    {
        emitter->update(deltaTime, particles);
    }

    // 移除不再活跃的发射器
    emitters.erase(std::remove_if(emitters.begin(), emitters.end(),
                                  [](const std::unique_ptr<BaseEmitter> &emitter)
                                  {
                                      return !emitter->getIsActive();
                                  }),
                   emitters.end());
}

/**
 * @brief 渲染发射器相关图形
 * @param window 渲染窗口
 *
 * 根据当前拖拽状态渲染相应的视觉元素：
 * - 如果正在拖拽，则渲染拖拽指示器（包括拖拽线和起始点）
 * - 如果没有拖拽，则不渲染任何内容
 */
void MouseEmitterController::render(sf::RenderWindow &window)
{
    if (isDragging)
    {
        renderDragIndicator(window);
    }
}

/**
 * @brief 创建点发射器
 * @param position 发射位置
 * @param direction 发射方向
 * @param speed 发射速度
 *
 * 创建一个新的点发射器实例，设置其位置、方向和速度参数，
 * 并配置粒子的质量、半径和颜色属性。发射器被激活后会立即发射粒子。
 * 创建完成后将发射器添加到发射器集合中。
 */
void MouseEmitterController::createPointEmitter(Vector2 position, Vector2 direction, float speed)
{
    auto emitter = std::make_unique<PointEmitter>(position, direction, speed, 1.0f, 4.0f, sf::Color(100, 200, 255));
    emitter->setActive(true); // 激活发射器，使其在下次更新时发射粒子
    emitters.push_back(std::move(emitter));
}

/**
 * @brief 设置暂停状态
 * @param paused 是否暂停
 */
void MouseEmitterController::setPaused(bool paused)
{
    isPaused = paused;
    std::cout << "[DEBUG] MouseEmitterController setPaused: " << (isPaused ? "true" : "false") << std::endl;
}

/**
 * @brief 获取暂停状态
 * @return 是否暂停
 */
bool MouseEmitterController::isControllerPaused() const
{
    return isPaused;
}

/**
 * @brief 渲染拖拽指示器
 * @param window 渲染窗口
 *
 * 渲染拖拽操作的视觉反馈，包括：
 * 1. 从拖拽起始位置到当前鼠标位置的直线（绿色到黄色渐变）
 * 2. 在拖拽起始位置绘制一个小圆点（绿色）
 * 这些视觉元素帮助用户了解拖拽的方向和距离，从而预测粒子的发射方向和速度
 */
void MouseEmitterController::renderDragIndicator(sf::RenderWindow &window) const
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    Vector2 currentPos(mousePos.x, mousePos.y);

    // 绘制拖拽线
    sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f(dragStartPosition.x, dragStartPosition.y), sf::Color::Green),
        sf::Vertex(sf::Vector2f(currentPos.x, currentPos.y), sf::Color::Yellow)};
    window.draw(line, 2, sf::Lines);

    // 绘制起始圆
    sf::CircleShape startCircle(3);
    startCircle.setPosition(dragStartPosition.x - 3, dragStartPosition.y - 3);
    startCircle.setFillColor(sf::Color::Green);
    window.draw(startCircle);
}