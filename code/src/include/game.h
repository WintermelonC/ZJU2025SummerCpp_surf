/**
 * @file game.h
 * @brief 游戏主头文件
 * 
 * 负责管理窗口、视图、主循环等
 */

#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "utils.h"
#include "player.h"

constexpr int WINDOW_WIDTH = 720;  // 窗口宽度
constexpr int WINDOW_HEIGHT = 1280;  // 窗口高度
constexpr int WINDOW_CENTER_X = WINDOW_WIDTH / 2;  // 窗口中心 X 坐标
constexpr int WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;  // 窗口中心 Y 坐标
constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子
constexpr float START_BUTTON_SCALE = 1.2f;  // 开始按钮缩放比例

enum class GameState {
    Start,  // 主菜单
    Playing,
    Paused,
    GameOver
};

class Game {
public:
    Game();
    ~Game() = default;

    void run();  // 游戏主循环

private:
    void handleEvents();  // 处理事件
    void handleMouseClick(const sf::Vector2f& mousePos);  // 处理鼠标点击事件

    void update();  // 更新游戏状态
    void updateView();  // 更新视图
    void updateBackground();  // 更新背景

    void render();  // 渲染游戏内容
    void renderStartMenu();  // 渲染开始菜单
    void renderPlayerAnimation();  // 渲染玩家动画

private:
    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟

    GameState m_state;  // 游戏状态
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;

    sf::RectangleShape m_bgShape;  // 背景形状

    Player m_player;  // 玩家对象

    sf::Clock m_animClock;  // 动画时钟
    int m_currentAnimFrame = 0; // 当前动画帧索引
};
