/**
 * @file game.h
 * @brief 游戏主头文件
 * 
 * 负责管理窗口、视图、主循环等
 */

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

constexpr int WINDOW_WIDTH = 1280;  // 窗口宽度
constexpr int WINDOW_HEIGHT = 1440;  // 窗口高度

class Game {
public:
    Game();
    ~Game() = default;

    // 游戏主循环
    void run();

private:
    // 处理事件
    void handleEvents();

    // 更新游戏状态
    void update();

    // 渲染游戏内容
    void render();  

private:
    sf::RenderWindow m_window;
    sf::View m_view;
    bool m_isRunning;
};
