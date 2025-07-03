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

    const sf::Vector2f& getVelocity() const { return m_velocity; }
    const sf::Angle& getAngle() const { return m_angle; }
    const PlayerState& getState() const { return m_state; }

    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setAngle(const sf::Angle& angle) { m_angle = angle; }

private:
    void updateState(const sf::Vector2f& mousePos);
    void updateYSpeed(const float deltaTime);
    void updateXSpeed(const float deltaTime);

private:
    const float m_acceleration1 = 10.f;
    const float m_acceleration2 = 50.f;
    const float m_maxSpeed = 50.f;
    const float m_XYScale1 = 0.4f;
    const float m_XYScale2 = 0.8f;
    const float m_powerScale = 1.5f;
    const float m_angle1 = 20.f;
    const float m_angle2 = 40.f;

    sf::Vector2f m_velocity = {0, 0};
    sf::Angle m_angle = sf::degrees(0.0f);
    PlayerState m_state = PlayerState::center;
    bool m_isPower = false;
};