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
#include "entity.h"
#include "obstacle.h"

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
    void handleMouseLeftClick();  // 处理鼠标左键点击事件
    void handleMouseRightClick();  // 处理鼠标右键点击事件

    void update();  // 更新游戏状态
    void updateView();  // 更新视图
    void updateBackground();  // 更新背景
    void updateObstacle();  // 更新障碍物
    void updateScore();  // 更新分数

    void render();  // 渲染游戏内容
    void renderStartMenu();  // 渲染开始菜单
    void renderPausedMenu();  // 渲染暂停菜单
    void renderPlayerAnimation();  // 渲染玩家动画
    void renderObstacle();  // 渲染障碍物

    void createObstacle();  // 新增随机障碍物
    void renderPlayerState();  // 渲染玩家状态（生命值、能量等）
    void renderScore();  // 渲染分数

private:
    const sf::Vector2i WINDOW_SIZE = {720, 1280};  // 窗口大小
    const float PARALLAX_FACTOR = 0.1f;  // 视差因子
    const sf::Vector2i BUTTON_SIZE = {180, 60};  // 按钮大小
    const sf::Vector2f START_BUTTON_POS = RENDER_CENTER_POS + sf::Vector2f(0.f, 200.f);  // 开始按钮坐标
    const sf::Vector2f START_BUTTON_SCALE = {1.2f, 1.2f};  // 开始按钮缩放比例
    const sf::Vector2f CONTINUE_BUTTON_POS = RENDER_CENTER_POS + sf::Vector2f(0.f, 200.f);  // 继续按钮坐标
    const sf::Vector2f CONTINUE_BUTTON_SCALE = {1.2f, 1.2f};  // 继续按钮缩放比例
    const sf::Vector2f RETURN_BUTTON_POS = RENDER_CENTER_POS + sf::Vector2f(0.f, 300.f);  // 返回按钮坐标
    const sf::Vector2f RETURN_BUTTON_SCALE = {0.8f, 0.8f};  // 返回按钮缩放比例
    const int HEART_X_OFFSET = 260;  // 生命值图标 X 坐标偏移量
    const int POWER_X_OFFSET = 100;  // 能量值图标 X 坐标偏移量
    const float HP_SCALE = 1.5f;  // 生命值图标缩放比例
    const float POWER_SCALE = HP_SCALE;  // 能量值图标缩放比例
    const float HP_GAP = 40.0f;  // 生命值图标之间的间隔
    const float POWER_GAP = HP_GAP;  // 能量值图标之间的间隔

    sf::RenderWindow m_window;  // 窗口
    sf::View m_view;  // 视图
    sf::Clock m_clock;  // 时钟
    sf::RectangleShape m_bgShape;  // 背景形状
    float m_score = 0.0f;  // 分数
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