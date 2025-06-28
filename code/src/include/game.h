/**
 * @file game.h
 * @brief 游戏主头文件
 * 
 * 负责管理窗口、视图、主循环等
 */

#pragma once

#include <string>
#include <queue>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "utils.h"
#include "player.h"
#include "entity.h"
#include "obstacle.h"

constexpr int WINDOW_WIDTH = 720;  // 窗口宽度
constexpr int WINDOW_HEIGHT = 1280;  // 窗口高度
constexpr int WINDOW_CENTER_X = WINDOW_WIDTH / 2;  // 窗口中心 X 坐标
constexpr int WINDOW_CENTER_Y = WINDOW_HEIGHT / 2;  // 窗口中心 Y 坐标
constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子
constexpr int START_BUTTON_WIDTH = 180;  // 开始按钮宽度
constexpr int START_BUTTON_HEIGHT = 60;  // 开始按钮高度
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
    void handleMouseLeftClick(const sf::Vector2f& mousePos);  // 处理鼠标左键点击事件

    void update();  // 更新游戏状态
    void updateView();  // 更新视图
    void updateBackground();  // 更新背景
    void updateObstacle();  // 更新障碍物

    void render();  // 渲染游戏内容
    void renderStartMenu();  // 渲染开始菜单
    void renderPausedMenu();  // 渲染暂停菜单
    void renderPlayerAnimation();  // 渲染玩家动画
    void renderObstacle();  // 渲染障碍物

    void createObstacle();  // 新增随机障碍物

private:
    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟
    sf::RectangleShape m_bgShape;  // 背景形状
    float m_offsetX = 0.0f;
    float m_offsetY = 0.0f;

    sf::Clock m_animClock;  // 动画时钟
    int m_currentAnimFrame = 0; // 当前动画帧索引
    sf::Clock m_obstacleSpawnClock; // 用于生成障碍物的时钟
    sf::Time m_spawnInterval = sf::seconds(1.5f); // 设置生成间隔为1.5秒

    GameState m_state;  // 游戏状态
    Player m_player;  // 玩家对象
    std::vector<Obstacle> m_obstacles;  // 障碍物队列
};