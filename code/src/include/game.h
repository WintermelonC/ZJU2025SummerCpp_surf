#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "config.h"
#include "player.h"
#include "eventBus.h"
#include "inputManager.h"
#include "renderSystem.h"

class Game {
public:
    Game();

    void run();

private:
    void processGameEvents();

    void update();
    void updateWater();

    void render();

private:
    const float PARALLAX_FACTOR = 0.1f;  // 视差因子

    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟

    EventBus m_eventBus;  // 事件总线
    InputManager m_inputManager;  // 输入管理器
    RenderSystem m_renderSystem;  // 渲染系统
    Player m_player;  // 玩家对象

    sf::Vector2f m_offset;  // 水面偏移量
};
