#pragma once

#include <string>
#include <array>
#include <cmath>
#include <vector>
#include <SFML/Graphics.hpp>
#include "utils.h"

constexpr float ACCELERATION_1 = 5.0f;  // 默认加速度
constexpr float ACCELERATION_2 = 10.0f;  // 增强加速度
constexpr float MAX_SPEED_1 = 25.0f;  // 默认最大速度
constexpr float MAX_SPEED_2 = 50.0f;  // 增强最大速度
constexpr float X_SPEED_1 = 5.0f;  // X 速度 1
constexpr float X_SPEED_2 = 10.0f;  // X 速度 2
constexpr float ANGLE_1 = 20.0f;  // 角度 1
constexpr float ANGLE_2 = 40.0f;  // 角度 2
constexpr float PLAYER_SCALE = 1.5f;  // 玩家缩放比例
constexpr int PLAYER_ANIM_FRAMES = 3;  // 玩家动画帧数
constexpr int PLAYER_WIDTH = 64;  // 玩家宽度
constexpr int PLAYER_HEIGHT = 96;  // 玩家高度
constexpr int PLAYER_X = RENDER_CENTER_X;  // 玩家初始 X 坐标
constexpr int PLAYER_Y = RENDER_HEIGHT / 5 * 2;  // 玩家初始 Y 坐标

enum class XState {
    CENTER,
    LEFT1,
    LEFT2,
    RIGHT1,
    RIGHT2
};

class Player {
public:
    Player(sf::Vector2f position = {PLAYER_X, PLAYER_Y});
    ~Player() = default;

    // 获取位置
    const sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
    // 获取精灵
    const sf::Sprite& getSprite() const { return m_sprite; }
    // 获取玩家速度
    const sf::Vector2f& getVelocity() const { return m_velocity; }
    // 获取玩家纹理路径
    const std::vector<Textures> getPaths() const;

    // 更新玩家状态
    void update(float dt, const sf::Vector2i& mousePosition, const sf::RenderWindow& window);
    // 初始化玩家状态
    void initial();

private:
    // 更新 X 速度
    void updateXSpeed(const sf::Vector2i& mousePosition, const sf::RenderWindow& window);
    // 更新 Y 速度
    void updateYSpeed(float dt);
    // 更新动画
    void updateAnimation(float dt);

private:
    sf::Sprite m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度
    float m_acceleration = ACCELERATION_1;  // 当前加速度
    float m_maxSpeed = MAX_SPEED_1;  // 当前最大速度
    
    int m_currentFrame = 0;  // 当前帧索引
    float m_animTimer = 0.0f;  // 动画计时器
    
    XState m_xState;  // 当前 X 状态
};
