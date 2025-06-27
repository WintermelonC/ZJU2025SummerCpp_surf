#include "include/player.h"
#include <iostream>

Player::Player(sf::Vector2f position)
    : m_texture(m_centerPaths[0]),
      m_sprite(m_texture),
      m_velocity({0.f, 0.f}),
      m_xState(XState::CENTER) {
    // 设置精灵中心点和位置
    m_sprite.setOrigin({m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f});
    m_sprite.setPosition(position);
}

void Player::update(float dt, const sf::Vector2i& mousePosition, const sf::Vector2u& windowSize) {
    updateXSpeed(mousePosition, windowSize);
    updateYSpeed(dt);
    updateAnimation(dt);
}

void Player::updateXSpeed(const sf::Vector2i& mousePosition, const sf::Vector2u& windowSize) {
    // x 速度根据鼠标位置调整
    float deltaX = mousePosition.x - windowSize.x / 2.f;
    float deltaY = mousePosition.y - windowSize.y / 2.f;
    sf::Angle angle = sf::radians(std::atan2(deltaX, deltaY));  // 计算角度
    if (deltaY >= 0.0f) {
        if (angle.asDegrees() >= ANGLE_2 || angle.asDegrees() <= -ANGLE_2) {
            m_velocity.x = X_SPEED_2 * (angle.asDegrees() >= 0.f ? 1.f : -1.f);
            m_xState = (angle.asDegrees() >= 0.f) ? XState::RIGHT2 : XState::LEFT2;
        } else if (angle.asDegrees() >= ANGLE_1 || angle.asDegrees() <= -ANGLE_1) {
            m_velocity.x = X_SPEED_1 * (angle.asDegrees() >= 0.f ? 1.f : -1.f);
            m_xState = (angle.asDegrees() >= 0.f) ? XState::RIGHT1 : XState::LEFT1;
        } else {
            m_velocity.x = 0.f;
            m_xState = XState::CENTER;
        }
    }
}

void Player::updateYSpeed(float dt) {
    // y 速度递增
    if (m_velocity.y < m_maxSpeed) {
        m_velocity.y += m_acceleration * dt;
        if (m_velocity.y > m_maxSpeed) {
            m_velocity.y = m_maxSpeed;  // 限制最大速度
        }
    }
}

void Player::updateAnimation(float dt) {
    // 动画更新逻辑
    const std::array<std::string, 3>& paths = 
        (m_xState == XState::CENTER) ? m_centerPaths :
        (m_xState == XState::LEFT1) ? m_left1Paths :
        (m_xState == XState::LEFT2) ? m_left2Paths :
        (m_xState == XState::RIGHT1) ? m_right1Paths :
        m_right2Paths;
    m_animTimer += dt;
    if (m_animTimer >= m_animInterval) {
        m_animTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % paths.size();
        if (!m_texture.loadFromFile(paths[m_currentFrame])) {
            throw std::runtime_error("Failed to load texture from " + paths[m_currentFrame]);
        }
        m_sprite.setTexture(m_texture);
    }
}
