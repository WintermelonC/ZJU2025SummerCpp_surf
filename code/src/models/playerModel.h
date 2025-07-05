#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "entityModel.h"
#include "../common/config.h"

enum class PlayerState {
    left2, left1, center, right1, right2, stop
};

class PlayerModel : public EntityModel {
public:
    PlayerModel();
    ~PlayerModel() = default;

    void update(const float deltaTime, const sf::Vector2f& mousePos);
    void usePower();
    
    //  重置功能
    void reset();

    const sf::Vector2f& getVelocity() const { return m_velocity; }
    const sf::Angle& getAngle() const { return m_angle; }
    const PlayerState& getState() const { return m_state; }
    const int& getHp() const { return m_hp; }
    const int& getPower() const { return m_power; }

    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setAngle(const sf::Angle& angle) { m_angle = angle; }

private:
    void updateState(const sf::Vector2f& mousePos);
    void updateAngle();
    void updateYSpeed(const float deltaTime);
    void updateXSpeed(const float deltaTime);
    void updatePower(const float& dt);  // 更新能量状态

private:
    const float m_acceleration1 = 10.f;
    const float m_acceleration2 = 50.f;
    const float m_maxSpeed = 50.f;
    const float m_XYScale1 = 0.4f;
    const float m_XYScale2 = 0.8f;
    const float m_powerScale = 1.5f;
    const float m_angle1 = 20.f;
    const float m_angle2 = 40.f;
    const float m_powerTime = 5.f;

    sf::Vector2f m_velocity = {0, 0};
    sf::Angle m_angle = sf::degrees(0.0f);
    PlayerState m_state = PlayerState::center;
    bool m_isPower = false;
    float m_powerTimer = 0.0f;  // 能量计时器
    int m_hp = Config::Player::PLAYER_HP;  // 玩家生命值
    int m_power = 0;  // 玩家能量值
};