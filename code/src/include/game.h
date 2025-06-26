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

constexpr int RENDER_WIDTH = 2560;  // 渲染宽度
constexpr int RENDER_HEIGHT = 1440;  // 渲染高度
constexpr int WINDOW_WIDTH = 540;  // 窗口宽度
constexpr int WINDOW_HEIGHT = 960;  // 窗口高度

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
    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    std::string m_backgroundPath{"../../assets/images/water_texture.png"};
    sf::Texture m_backgroundTexture;  // 背景纹理
    sf::Sprite m_background;  // 背景精灵
    bool m_isRunning;
};
