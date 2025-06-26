#include "include/player.h"

Player::Player(sf::Vector2f position)
    : m_texture(m_path),
      m_sprite(m_texture),
      m_velocity({0.f, 0.f}) {
    // 设置精灵中心点和位置
    m_sprite.setOrigin({m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f});
    m_sprite.setPosition(position);
}

void Player::update(float dt) {
    // 速度递增
    if (m_velocity.y < m_maxSpeed) {
        m_velocity.y += m_acceleration * dt;
        if (m_velocity.y > m_maxSpeed) {
            m_velocity.y = m_maxSpeed;  // 限制最大速度
        }
    }
}
