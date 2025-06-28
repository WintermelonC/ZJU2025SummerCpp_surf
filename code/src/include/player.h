#pragma once

#include <string>
#include <array>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "utils.h"

constexpr sf::Vector2f PLAYER_POS = {RENDER_CENTER_POS.x, RENDER_SIZE.y / 5 * 2};  // 玩家初始坐标
constexpr sf::Vector2i PLAYER_SIZE = {64, 96};  // 玩家尺寸
constexpr int PLAYER_HP = 3;  // 玩家最大生命值
constexpr int PLAYER_POWER = 3;  // 玩家最大能量值

enum class PlayerState {
    Center,
    Left1,
    Left2,
    Right1,
    Right2,
    Stop
};

class Player {
public:
    Player(sf::Vector2f position = PLAYER_POS);
    ~Player() = default;

    // 获取位置
    const sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
    // 获取精灵
    const sf::Sprite& getSprite() const { return m_sprite; }
    // 获取玩家速度
    const sf::Vector2f& getVelocity() const { return m_velocity; }
    // 获取玩家纹理路径
    const std::vector<Textures> getPaths() const;
    const int getHP() const { return m_hp; }  // 获取玩家生命值
    const int getPower() const { return m_power; }  // 获取玩家能量值
    const bool isAccelerating() const { return m_isAccelerating; }  // 是否正在加速
    const bool isTrun() const { return m_isTurn; }  // 是否转弯

    void usePower();  // 使用能量加速

    void update(const float& dt, const sf::RenderWindow& window);  // 更新玩家状态
    void initial();  // 初始化玩家状态

    // 碰撞检测
    void onCollision(); // 被Game类调用，用于触发碰撞反应
    // 设置无敌状态
    bool isInvincible() const { return m_isInvincible; }

private:
    void updateXSpeed();  // 更新 X 速度
    void updateYSpeed(const float& dt);  // 更新 Y 速度 
    void updateAnimation(const float& dt);  // 更新动画
    void updateState(const sf::RenderWindow& window);  // 更新玩家状态
    void updatePower(const float& dt);  // 更新能量状态
    void updateTurn();  // 更新转弯状态

private:
    const float PLAYER_SCALE = 1.5f;  // 玩家缩放比例
    const int POWER_TIME = 5;  // 能量持续时间（秒）
    const float ACCELERATION_1 = 10.0f;  // 默认加速度
    const float ACCELERATION_2 = 50.0f;  // 增强加速度
    const float ACCELERATION_3 = 50.0f;  // 减速加速度
    const float MAX_SPEED = 50.0f;  // Y 最大速度
    const float XY_SPEED_1 = 0.4f;  // Y / X 比例 1
    const float XY_SPEED_2 = 0.8f;  // Y / X 比例 2
    const float SPEED_SCALE = 2.0f;  // 速度加速比例
    const float ANGLE_1 = 20.0f;  // 角度 1
    const float ANGLE_2 = 40.0f;  // 角度 2

    sf::Sprite m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度
    PlayerState m_state;  // 当前状态
    PlayerState m_lastState;  // 上一个状态
    bool m_isTurn = false;  // 是否转弯
    int m_hp = PLAYER_HP;  // 玩家生命值
    int m_power = 3;  // 玩家能量值
    
    int m_currentFrame = 0;  // 当前帧索引
    float m_animTimer = 0.0f;  // 动画计时器
    
    bool m_isInvincible = false;    // 是否处于无敌（虚化）状态
    bool m_isPaused = false;        // 是否处于碰撞后的停顿状态

    sf::Clock m_invincibilityTimer; // 无敌状态的计时器
    sf::Clock m_pauseTimer;         // 碰撞后停顿的计时器

    const sf::Time INVINCIBILITY_DURATION = sf::seconds(3.0f); // 无敌总时长
    const sf::Time PAUSE_DURATION = sf::seconds(1.0f);         // 停顿时长
    bool m_isAccelerating = false;  // 是否正在加速
    float m_powerTimer = 0.0f;  // 能量计时器
};
