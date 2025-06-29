#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "entityManager.h"
#include "config.h"

#ifdef DEBUG
#include <iostream>
#endif  // DEBUG

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
    Player();

    void update(const float& dt, const sf::Vector2f& mousePos);
    void usePower();

    const sf::Vector2f& getVelocity() const { return m_velocity; }  // 获取玩家速度
    const bool isTrun() const { return m_isTurn; }  // 是否转弯
    const bool isPower() const { return m_isPower; }  // 是否使用能量
    const sf::Angle getAngle() const {
        switch (m_state) {
            case PlayerState::Left1:
            case PlayerState::Right1:
                return sf::degrees(m_velocity.x >= 0.f ? ANGLE_1 : -ANGLE_1);
            case PlayerState::Left2:
            case PlayerState::Right2:
                return sf::degrees(m_velocity.x >= 0.f ? ANGLE_2 : -ANGLE_2);
        }
        return sf::degrees(0.f);
    }
    const int getHP() const { return m_hp; }  // 获取玩家生命值
    const int getPower() const { return m_power; }  // 获取玩家能量

    const sf::Vector2f& getVelocity() const { return m_velocity; }  // 获取玩家速度

private:
    void updateState(const sf::Vector2f& mousePos);  // 更新玩家状态
    void updateXSpeed();  // 更新 X 轴速度
    void updateYSpeed(const float& dt);  // 更新 Y 轴速度
    void updateAnimation(const float& dt);  // 更新动画
    void updatePower(const float& dt);  // 更新能量状态
    void updateTurn();  // 更新转弯状态

private:
    const sf::Vector2f SCALE = {1.5f, 1.5f};  // 玩家缩放比例
    const float ACCELERATION_1 = 10.0f;  // 加速度 1
    const float ACCELERATION_2 = 50.0f;  // 加速度 2
    const float MAX_SPEED = 50.0f;  // Y 最大速度
    const float XY_SPEED_1 = 0.4f;  // Y / X 比例 1
    const float XY_SPEED_2 = 0.8f;  // Y / X 比例 2
    const float ANGLE_1 = 20.0f;  // 角度 1
    const float ANGLE_2 = 40.0f;  // 角度 2
    const int POWER_TIME = 5;  // 能量持续时间（秒）
    const float SPEED_SCALE = 1.5f;  // 速度加速比例

    sf::Sprite& m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度
    PlayerState m_state;  // 玩家状态
    PlayerState m_lastState;  // 上一个状态
    bool m_isTurn = false;  // 是否转弯
    int m_hp = Config::Player::PLAYER_HP;  // 玩家生命值
    int m_power = 0;  // 玩家能量值
    bool m_isPower = false;  // 玩家是否使用能量
    float m_powerTimer = 0.0f;  // 能量计时器

    int m_currentFrame = 0;  // 当前动画帧
    float m_animTimer = 0.0f;  // 动画计时器
};
