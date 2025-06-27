#include "include/player.h"

Player::Player(sf::Vector2f position)
    : m_sprite(Utils::renderSprite(Textures::player_center_1, sf::Color::White, position, {PLAYER_SCALE, PLAYER_SCALE}, true, false)),
      m_velocity({0.f, 0.f}),
      m_xState(XState::CENTER) {}

const std::vector<Textures> Player::getPaths() const { 
    return {
        Textures::player_left_21, Textures::player_left_22, Textures::player_left_23,
        Textures::player_left_11, Textures::player_left_12, Textures::player_left_13,
        Textures::player_center_1, Textures::player_center_2, Textures::player_center_3,
        Textures::player_right_11, Textures::player_right_12, Textures::player_right_13,
        Textures::player_right_21, Textures::player_right_22, Textures::player_right_23,
        Textures::player_right_11, Textures::player_right_12, Textures::player_right_13,
        Textures::player_center_1, Textures::player_center_2, Textures::player_center_3,
        Textures::player_left_11, Textures::player_left_12, Textures::player_left_13
    };
}

void Player::update(float dt, const sf::Vector2i& mousePosition, const sf::RenderWindow& window) {
    updateXSpeed(mousePosition, window);
    updateYSpeed(dt);
    updateAnimation(dt);
}

void Player::updateXSpeed(const sf::Vector2i& mousePosition, const sf::RenderWindow& window) {
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
    // x 速度根据鼠标位置调整
    float deltaX = worldPos.x - m_sprite.getPosition().x;
    float deltaY = worldPos.y - m_sprite.getPosition().y;
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
    const float m_animInterval = 0.1f;  // 动画间隔时间
    std::array<Textures, PLAYER_ANIM_FRAMES> paths;
    switch (m_xState) {
        case XState::CENTER:
            paths = {Textures::player_center_1, Textures::player_center_2, Textures::player_center_3};
            break;
        case XState::LEFT1:
            paths = {Textures::player_left_11, Textures::player_left_12, Textures::player_left_13};
            break;
        case XState::LEFT2:
            paths = {Textures::player_left_21, Textures::player_left_22, Textures::player_left_23};
            break;
        case XState::RIGHT1:
            paths = {Textures::player_right_11, Textures::player_right_12, Textures::player_right_13};
            break;
        case XState::RIGHT2:
            paths = {Textures::player_right_21, Textures::player_right_22, Textures::player_right_23};
            break;
    }

    m_animTimer += dt;
    if (m_animTimer >= m_animInterval) {
        m_animTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % PLAYER_ANIM_FRAMES;
        m_sprite.setTexture(*Utils::getTexture(paths[m_currentFrame]));
    }
}
