#include "include/player.h"

Player::Player(sf::Vector2f position)
    : m_sprite(Utils::renderSprite(Textures::player_center_1, sf::Color::White, position, {PLAYER_SCALE, PLAYER_SCALE}, false)),
      m_velocity({0.f, 0.f}),
      m_xState(XState::Center) {}

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

void Player::update(float dt, const sf::RenderWindow& window) {
    const sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
    const sf::Vector2f worldPos = window.mapPixelToCoords(mousePosition);
    const float deltaX = worldPos.x - m_sprite.getPosition().x;
    const float deltaY = worldPos.y - m_sprite.getPosition().y;
    const sf::Angle angle = sf::radians(std::atan2(deltaX, deltaY));  // 计算角度
    if (deltaY >= 0.0f) {
        if (angle.asDegrees() >= ANGLE_2 || angle.asDegrees() <= -ANGLE_2) {
            m_xState = (angle.asDegrees() >= 0.f) ? XState::Right2 : XState::Left2;
        } else if (angle.asDegrees() >= ANGLE_1 || angle.asDegrees() <= -ANGLE_1) {
            m_xState = (angle.asDegrees() >= 0.f) ? XState::Right1 : XState::Left1;
        } else {
            m_xState = XState::Center;
        }
    } else {
        // 减速
        m_xState = XState::Stop;
    }
    updateXSpeed();
    updateYSpeed(dt);
    updateAnimation(dt);
}

void Player::initial() {
    m_velocity = {0.f, 0.f};
    m_acceleration = ACCELERATION_1;
    m_hp = PLAYER_HP;
    m_power = 0;
    m_maxSpeed = MAX_SPEED_1;
    m_xState = XState::Center;
    m_currentFrame = 0;
    m_animTimer = 0.f;
    m_sprite.setTexture(*Utils::getTexture(Textures::player_center_1));
    m_sprite.setScale({PLAYER_SCALE, PLAYER_SCALE});
}

void Player::updateXSpeed() {
    switch (m_xState) {
        case XState::Center:
            m_velocity.x = 0.f;
            break;
        case XState::Left1:
            m_velocity.x = -X_SPEED_1;  // 向左移动
            break;
        case XState::Left2:
            m_velocity.x = -X_SPEED_2;  // 向左移动
            break;
        case XState::Right1:
            m_velocity.x = X_SPEED_1;  // 向右移动
            break;
        case XState::Right2:
            m_velocity.x = X_SPEED_2;  // 向右移动
            break;
        case XState::Stop:
            m_velocity.x = 0.f;  // 停止移动
            break;
    }
}

void Player::updateYSpeed(float dt) {
    switch (m_xState) {
        case XState::Center:
        case XState::Left1:
        case XState::Left2:
        case XState::Right1:
        case XState::Right2:
            if (m_velocity.y < m_maxSpeed) {
                m_velocity.y += m_acceleration * dt;  // 增加速度
                if (m_velocity.y > m_maxSpeed) {
                    m_velocity.y = m_maxSpeed;  // 限制最大速度
                }
            }
            break;
        case XState::Stop:
            if (m_velocity.y > 0.f) {
                m_velocity.y -= ACCELERATION_2 * dt;  // 减少速度
                if (m_velocity.y < 0.f) {
                    m_velocity.y = 0.f;  // 限制最小速度
                }
            }
            break;
    }
}

void Player::updateAnimation(float dt) {
    // 动画更新逻辑
    const float m_animInterval = 0.1f;  // 动画间隔时间
    std::array<Textures, PLAYER_ANIM_FRAMES> paths;
    switch (m_xState) {
        case XState::Center:
            paths = {Textures::player_center_1, Textures::player_center_2, Textures::player_center_3};
            break;
        case XState::Left1:
            paths = {Textures::player_left_11, Textures::player_left_12, Textures::player_left_13};
            break;
        case XState::Left2:
            paths = {Textures::player_left_21, Textures::player_left_22, Textures::player_left_23};
            break;
        case XState::Right1:
            paths = {Textures::player_right_11, Textures::player_right_12, Textures::player_right_13};
            break;
        case XState::Right2:
            paths = {Textures::player_right_21, Textures::player_right_22, Textures::player_right_23};
            break;
        case XState::Stop:
            paths = {Textures::player_stop_1, Textures::player_stop_2, Textures::player_stop_3};
            break;
    }

    m_animTimer += dt;
    if (m_animTimer >= m_animInterval) {
        m_animTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % PLAYER_ANIM_FRAMES;
        m_sprite.setTexture(*Utils::getTexture(paths[m_currentFrame]));
    }
}
