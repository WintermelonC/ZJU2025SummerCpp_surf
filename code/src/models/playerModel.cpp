#include "playerModel.h"

PlayerModel::PlayerModel()
    : EntityModel({1280, 576}, {64, 96}) {
    setCollisionBox(
        position,
        size / 2.f,
        size,
        sf::degrees(0.0f)
    );
}

void PlayerModel::update(const float deltaTime, const sf::Vector2f& mousePos) {
    updateState(mousePos);
    updateYSpeed(deltaTime);
    updateXSpeed(deltaTime);
}

void PlayerModel::updateState(const sf::Vector2f& mousePos) {
    const sf::Vector2f delta = mousePos - position;
    const sf::Angle angle = sf::radians(std::atan2(delta.x, delta.y));
    
    if (delta.y >= 0.0f) {
        const float angleDeg = angle.asDegrees();
        if (angleDeg >= m_angle2 || angleDeg <= -m_angle2) {
            m_state = (angleDeg >= 0.f) ? PlayerState::right2 : PlayerState::left2;
            m_angle = sf::degrees((angleDeg >= 0.f) ? m_angle2 : -m_angle2);
        } else if (angleDeg >= m_angle1 || angleDeg <= -m_angle1) {
            m_state = (angleDeg >= 0.f) ? PlayerState::right1 : PlayerState::left1;
            m_angle = sf::degrees((angleDeg >= 0.f) ? m_angle1 : -m_angle1);
        } else {
            m_state = PlayerState::center;
            m_angle = sf::degrees(0.0f);
        }
    } else {
        m_state = PlayerState::stop;
        m_angle = sf::degrees(0.0f);
    }
}

void PlayerModel::updateYSpeed(const float deltaTime) {
    if (m_state == PlayerState::stop) {
        // 减速
        m_velocity.y = std::max(0.0f, m_velocity.y - m_acceleration2 * deltaTime);
    } else {
        // 加速
        float acceleration = m_acceleration1;
        float maxSpeed = m_maxSpeed;
        if (m_isPower) {
            acceleration = m_acceleration1;
            maxSpeed = m_maxSpeed * m_powerScale;
        }
        
        m_velocity.y = std::min(maxSpeed, m_velocity.y + acceleration * deltaTime);
    }
}

void PlayerModel::updateXSpeed(const float deltaTime) {
    switch (m_state) {
        case PlayerState::center:
            m_velocity.x = 0.0f;
            break;
        case PlayerState::left1:
            m_velocity.x = -m_velocity.y * m_XYScale1;
            break;
        case PlayerState::left2:
            m_velocity.x = -m_velocity.y * m_XYScale2;
            break;
        case PlayerState::right1:
            m_velocity.x = m_velocity.y * m_XYScale1;
            break;
        case PlayerState::right2:
            m_velocity.x = m_velocity.y * m_XYScale2;
            break;
        case PlayerState::stop:
            m_velocity.x = 0.0f;
            break;
    }
}
