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

class Game {
public:
    Game();
    ~Game() = default;

    void run();  // 游戏主循环

private:
    void handleEvents();  // 处理事件
    void update();  // 更新游戏状态
    void render();  // 渲染游戏内容

    void updateView();  // 更新视图
    void updateBackground();  // 更新背景

private:
    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    
    std::string m_bgPath{"../../assets/images/water_texture.png"};
    sf::Texture m_bgTexture;  // 背景纹理
    sf::Sprite m_bgSprite;  // 背景精灵

    Player m_player;  // 玩家对象
};
