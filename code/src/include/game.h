/**
 * @file game.h
 * @brief 游戏主头文件
 * 
 * 负责管理窗口、视图、主循环等
 */

#pragma once

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "utils.h"
#include "player.h"

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
    void handleMouseLeftClick(const sf::Vector2f& mousePos);  // 处理鼠标左键点击事件
    void handleMouseRightClick(const sf::Vector2f& mousePos);  // 处理鼠标右键点击事件

    void update();  // 更新游戏状态
    void updateView();  // 更新视图
    void updateBackground();  // 更新背景

    void render();  // 渲染游戏内容
    void renderStartMenu();  // 渲染开始菜单
    void renderPausedMenu();  // 渲染暂停菜单
    void renderPlayerAnimation();  // 渲染玩家动画
    void renderPlayerState();  // 渲染玩家状态（生命值、能量等）

private:
    const int WINDOW_WIDTH = 720;  // 窗口宽度
    const int WINDOW_HEIGHT = 1280;  // 窗口高度
    const float PARALLAX_FACTOR = 0.1f;  // 视差因子
    const int BUTTON_WIDTH = 180;  // 按钮宽度
    const int BUTTON_HEIGHT = 60;  // 按钮高度
    const float START_BUTTON_X = RENDER_CENTER_X;  // 开始按钮 X 坐标
    const float START_BUTTON_Y = RENDER_CENTER_Y + 200;  // 开始按钮 Y 坐标
    const float START_BUTTON_SCALE = 1.2f;  // 开始按钮缩放比例
    const float CONTINUE_BUTTON_X = RENDER_CENTER_X;  // 继续按钮 X 坐标
    const float CONTINUE_BUTTON_Y = RENDER_CENTER_Y + 200;  // 继续按钮 Y 坐标
    const float CONTINUE_BUTTON_SCALE = 1.2f;  // 继续按钮缩放比例
    const float RETURN_BUTTON_X = RENDER_CENTER_X;  // 返回按钮 X 坐标
    const float RETURN_BUTTON_Y = RENDER_CENTER_Y + 300;  // 返回按钮 Y 坐标
    const float RETURN_BUTTON_SCALE = 0.8f;  // 返回按钮缩放比例
    const int HEART_X_OFFSET = 300;  // 生命值图标 X 坐标偏移量
    const int POWER_X_OFFSET = 100;  // 能量值图标 X 坐标偏移量

    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟
    sf::RectangleShape m_bgShape;  // 背景形状
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;

    sf::Clock m_animClock;  // 动画时钟
    int m_currentAnimFrame = 0; // 当前动画帧索引

    GameState m_state;  // 游戏状态
    Player m_player;  // 玩家对象
};
