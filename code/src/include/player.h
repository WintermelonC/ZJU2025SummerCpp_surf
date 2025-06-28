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

    const sf::Vector2f& getVelocity() const { return m_velocity; }  // 获取玩家速度

private:
    void updateState(const sf::Vector2f& mousePos);  // 更新玩家状态
    void updateXSpeed();  // 更新 X 轴速度
    void updateYSpeed(const float& dt);  // 更新 Y 轴速度
    void updateAnimation(const float& dt);  // 更新动画

private:
    const sf::Vector2f POSITION = {
        Config::Window::RENDER_CENTER.x, 
        Config::Window::RENDER_SIZE.y / 5.0f * 2.0f
    };  // 玩家初始位置
    const sf::Vector2f SCALE = {1.5f, 1.5f};  // 玩家缩放比例
    const float ACCELERATION_1 = 10.0f;  // 加速度 1
    const float ACCELERATION_2 = 50.0f;  // 加速度 2
    const float MAX_SPEED = 50.0f;  // Y 最大速度
    const float XY_SPEED_1 = 0.4f;  // Y / X 比例 1
    const float XY_SPEED_2 = 0.8f;  // Y / X 比例 2
    const float ANGLE_1 = 20.0f;  // 角度 1
    const float ANGLE_2 = 40.0f;  // 角度 2

    sf::Sprite& m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度
    PlayerState m_state;  // 玩家状态

    int m_currentFrame = 0;  // 当前动画帧
    float m_animTimer = 0.0f;  // 动画计时器
};
