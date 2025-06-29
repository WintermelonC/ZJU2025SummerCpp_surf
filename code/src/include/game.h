#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "config.h"
#include "player.h"
#include "eventBus.h"
#include "inputManager.h"
#include "renderSystem.h"

enum class GameState {
    StartMenu,
    Playing,
    Paused,
    GameOver
};

class Game {
public:
    Game();

    void run();

private:
    void processGameEvents();

    void update();
    void updateWater();
    void updateScore();  // 更新分数

    void render();

private:
    const float SPEED_THRESHOLD_1 = 20.0f;  // 速度超过阈值，产生水波
    const float SPEED_THRESHOLD_2 = 40.0f;  // 速度超过阈值，产生水波

    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟
    GameState m_gameState;  // 游戏状态

    EventBus m_eventBus;  // 事件总线
    InputManager m_inputManager;  // 输入管理器
    RenderSystem m_renderSystem;  // 渲染系统
    Player m_player;  // 玩家对象
    float m_score = 0.0f;  // 分数

    sf::Vector2f m_offset;  // 水面偏移量
};
