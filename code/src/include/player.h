#pragma once

#include <string>
#include <array>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "utils.h"

constexpr float ACCELERATION_1 = 5.0f;  // 默认加速度
constexpr float ACCELERATION_2 = 10.0f;  // 增强加速度
constexpr float MAX_SPEED_1 = 50.0f;  // 默认最大速度
constexpr float MAX_SPEED_2 = 100.0f;  // 增强最大速度
constexpr float X_SPEED_1 = 5.0f;  // X 速度 1
constexpr float X_SPEED_2 = 10.0f;  // X 速度 2
constexpr float ANGLE_1 = 15.0f;  // 角度 1
constexpr float ANGLE_2 = 30.0f;  // 角度 2

enum class XState {
    CENTER,
    LEFT1,
    LEFT2,
    RIGHT1,
    RIGHT2
};

class Player {
public:
    Player(sf::Vector2f position = {WINDOW_CENTER_X, WINDOW_CENTER_Y});
    ~Player() = default;

    const sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
    const sf::Sprite& getSprite() const { return m_sprite; }
    const sf::Vector2f& getVelocity() const { return m_velocity; }

    // 更新玩家速度
    void update(float dt, const sf::Vector2i& mousePosition, const sf::Vector2u& windowSize);

private:
    void updateXSpeed(const sf::Vector2i& mousePosition, const sf::Vector2u& windowSize);
    void updateYSpeed(float dt);
    void updateAnimation(float dt);

private:
    const std::array<std::string, 3> m_centerPaths {
        "../../assets/images/player/player_center_1.png",
        "../../assets/images/player/player_center_2.png",
        "../../assets/images/player/player_center_3.png"
    };
    const std::array<std::string, 3> m_left1Paths {
        "../../assets/images/player/player_left_11.png",
        "../../assets/images/player/player_left_12.png",
        "../../assets/images/player/player_left_13.png"
    };
    const std::array<std::string, 3> m_left2Paths {
        "../../assets/images/player/player_left_21.png",
        "../../assets/images/player/player_left_22.png",
        "../../assets/images/player/player_left_23.png"
    };
    const std::array<std::string, 3> m_right1Paths {
        "../../assets/images/player/player_right_11.png",
        "../../assets/images/player/player_right_12.png",
        "../../assets/images/player/player_right_13.png"
    };
    const std::array<std::string, 3> m_right2Paths {
        "../../assets/images/player/player_right_21.png",
        "../../assets/images/player/player_right_22.png",
        "../../assets/images/player/player_right_23.png"
    };
    sf::Texture m_texture;  // 玩家纹理
    sf::Sprite m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度

    int m_currentFrame = 0;  // 当前帧索引
    float m_animTimer = 0.0f;  // 动画计时器
    const float m_animInterval = 0.1f;  // 动画间隔时间
    float m_acceleration = ACCELERATION_1;  // 当前加速度
    float m_maxSpeed = MAX_SPEED_1;  // 当前最大速度
    XState m_xState;  // 当前 X 状态
};
