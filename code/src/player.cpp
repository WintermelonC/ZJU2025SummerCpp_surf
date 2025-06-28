#include "player.h"

Player::Player() 
    : m_sprite(EntityManager::getSprite(EntityType::player)),
      m_velocity({0.0f, 0.0f}),
      m_state(PlayerState::Center) {
    EntityManager::setSprite(EntityType::player, POSITION, SCALE);  // 设置玩家精灵位置和缩放
};

void Player::update(const float& dt, const sf::Vector2f& mousePos) {
    updateState(mousePos);  // 更新玩家状态
    updateYSpeed(dt);  // 更新 Y 轴速度
    updateXSpeed();  // 更新 X 轴速度
    updateAnimation(dt);  // 更新动画
}

void Player::updateState(const sf::Vector2f& mousePos) {
    const sf::Vector2f delta = mousePos - m_sprite.getPosition();
    const sf::Angle angle = sf::radians(std::atan2(delta.x, delta.y));  // 计算鼠标与玩家位置的角度
    if (delta.y >= 0.0f) {
        if (angle.asDegrees() >= ANGLE_2 || angle.asDegrees() <= -ANGLE_2) {
            m_state = (angle.asDegrees() >= 0.f) ? PlayerState::Right2 : PlayerState::Left2;
        } else if (angle.asDegrees() >= ANGLE_1 || angle.asDegrees() <= -ANGLE_1) {
            m_state = (angle.asDegrees() >= 0.f) ? PlayerState::Right1 : PlayerState::Left1;
        } else {
            m_state = PlayerState::Center;
        }
    } else {
        // 减速
        m_state = PlayerState::Stop;
    }
}

void Player::updateXSpeed() {
    switch (m_state) {
        case PlayerState::Center:
            m_velocity.x = 0.f;
            break;
        case PlayerState::Left1:
            m_velocity.x = m_velocity.y * -XY_SPEED_1;  // 左侧速度
            break;
        case PlayerState::Left2:
            m_velocity.x = m_velocity.y * -XY_SPEED_2;  // 左侧速度
            break;
        case PlayerState::Right1:
            m_velocity.x = m_velocity.y * XY_SPEED_1;  // 右侧速度
            break;
        case PlayerState::Right2:
            m_velocity.x = m_velocity.y * XY_SPEED_2;  // 右侧速度
            break;
        case PlayerState::Stop:
            m_velocity.x = 0.f;  // 停止移动
            break;
    }
}

void Player::updateYSpeed(const float& dt) {
    switch (m_state) {
        case PlayerState::Center:
        case PlayerState::Left1:
        case PlayerState::Left2:
        case PlayerState::Right1:
        case PlayerState::Right2:
            if (m_velocity.y < MAX_SPEED) {
                m_velocity.y += ACCELERATION_1 * dt;  // 增加 Y 轴速度
                if (m_velocity.y > MAX_SPEED) {
                    m_velocity.y = MAX_SPEED;  // 限制 Y 轴速度不超过最大值
                }
            }
            break;
        case PlayerState::Stop:
            if (m_velocity.y > 0) {
                m_velocity.y -= ACCELERATION_2 * dt;
                if (m_velocity.y < 0) {
                    m_velocity.y = 0;  // 限制 Y 轴速度不低于 0
                }
            }
            break;
    }

#ifdef DEBUG
    // std::cout << "Player velocity: (" << m_velocity.x << ", " << m_velocity.y << ")" << std::endl;
#endif  // DEBUG
}

void Player::updateAnimation(const float& dt) {
    const float m_animInterval = 0.1f;  // 动画间隔时间
    std::vector<Textures> paths;
    switch (m_state) {
        case PlayerState::Center:
            paths = {Textures::player_center_1, Textures::player_center_2, Textures::player_center_3};
            break;
        case PlayerState::Left1:
            paths = {Textures::player_left_11, Textures::player_left_12, Textures::player_left_13};
            break;
        case PlayerState::Left2:
            paths = {Textures::player_left_21, Textures::player_left_22, Textures::player_left_23};
            break;
        case PlayerState::Right1:
            paths = {Textures::player_right_11, Textures::player_right_12, Textures::player_right_13};
            break;
        case PlayerState::Right2:
            paths = {Textures::player_right_21, Textures::player_right_22, Textures::player_right_23};
            break;
        case PlayerState::Stop:
            paths = {Textures::player_stop_1, Textures::player_stop_2, Textures::player_stop_3};
            break;
    }

    m_animTimer += dt;
    if (m_animTimer >= m_animInterval) {
        m_animTimer = 0.f;
        m_currentFrame = (m_currentFrame + 1) % paths.size();
        EntityManager::setSpriteTexture(EntityType::player, paths[m_currentFrame]);  // 更新玩家精灵纹理
    }
}
