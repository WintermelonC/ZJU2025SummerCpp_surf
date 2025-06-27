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

constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子

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
    void update();  // 更新游戏状态

    void render();  // 渲染游戏内容
    void renderStartMenu();  // 渲染开始菜单
    sf::Text renderText(
        const sf::Font& font, 
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter = true,
        const bool ifCovert = false
    );

    void updateView();  // 更新视图
    void updateBackground();  // 更新背景

private:
    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟
    sf::Vector2i m_mousePosition;  // 鼠标位置
    GameState m_state;  // 游戏状态
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;

    std::string m_bgPath{"../../assets/images/water_texture.png"};
    sf::Texture m_bgTexture;  // 背景纹理
    sf::RectangleShape m_bgShape;  // 背景形状
    std::string m_JHPath{"../../assets/fonts/MSJHBD.TTC"};
    sf::Font m_fontJH;  // 字体对象
    std::string m_almmPath{"../../assets/fonts/almmdfdk.TTF"};
    sf::Font m_fontAlmm;  // 字体对象

    Player m_player;  // 玩家对象
};
