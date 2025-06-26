#include "include/player.h"

Player::Player(sf::Vector2f position)
    : m_texture(m_path),
      m_sprite(m_texture) {
    // 设置精灵中心点和位置
    m_sprite.setOrigin({m_texture.getSize().x / 2.f, m_texture.getSize().y / 2.f});
    m_sprite.setPosition(position);
}
