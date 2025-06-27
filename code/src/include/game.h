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

    /**
     * @brief 渲染文本
     * 
     * @param font 字体对象
     * @param content 文本内容
     * @param size 字体大小
     * @param color 文本颜色
     * @param position 文本位置
     * @param ifCenter 是否将 Origin 设置为中心
     * @param ifCovert 是否转换为宽文本
     * @return 渲染后的文本对象
     * 
     * @warning 如果为中文字符，必须将 ifCovert 设置为 true
     */
    sf::Text renderText(
        const sf::Font& font, 
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter = true,
        const bool ifCovert = false
    );

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
    sf::Font m_fontJH;  // Microsoft JhengHei
    std::string m_almmPath{"../../assets/fonts/almmdfdk.TTF"};
    sf::Font m_fontAlmm;  // 阿里妈妈东方大楷

    Player m_player;  // 玩家对象

    sf::Clock m_animClock;  // 动画时钟
    int m_currentAnimFrame = 0; // 当前动画帧索引
};
