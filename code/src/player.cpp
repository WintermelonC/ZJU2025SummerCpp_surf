#include "include/player.h"

Player::Player(sf::Vector2f position)
    : m_sprite(Utils::renderSprite(Textures::player_center_1, sf::Color::White, position, {PLAYER_SCALE, PLAYER_SCALE}, false)),
      m_velocity({0.f, 0.f}),
      m_state(PlayerState::Center) {}

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
    if (m_isAccelerating) {
        m_powerTimer += dt;  // 增加能量计时器
        if (m_powerTimer >= POWER_TIME) {
            m_isAccelerating = false;  // 停止加速
            m_powerTimer = 0.0f;  // 重置能量计时器
        }
    } else {
        m_powerTimer = 0.0f;  // 重置能量计时器
    }
    updateXSpeed(dt);
    updateYSpeed(dt);
    updateAnimation(dt);
}

void Player::initial() {
    m_velocity = {0.f, 0.f};
    m_hp = PLAYER_HP;
    m_power = 3;
    m_state = PlayerState::Center;
    m_currentFrame = 0;
    m_animTimer = 0.f;
    m_sprite.setTexture(*Utils::getTexture(Textures::player_center_1));
    m_sprite.setScale({PLAYER_SCALE, PLAYER_SCALE});
}

void Player::usePower() {
    if (m_power > 0) {
        m_power--;  // 使用能量
        m_powerTimer = 0.0f;
        m_isAccelerating = true;  // 开始加速
    }
}

void Player::updateXSpeed(float dt) {
    switch (m_state) {
        case PlayerState::Center:
            m_velocity.x = 0.f;
            break;
        case PlayerState::Left1:
            m_velocity.x = m_velocity.y * (m_isAccelerating ? SPEED_SCALE : 1.f) * -XY_SPEED_1;  // 左侧速度
            break;
        case PlayerState::Left2:
            m_velocity.x = m_velocity.y * (m_isAccelerating ? SPEED_SCALE : 1.f) * -XY_SPEED_2;  // 左侧速度
            break;
        case PlayerState::Right1:
            m_velocity.x = m_velocity.y * (m_isAccelerating ? SPEED_SCALE : 1.f) * XY_SPEED_1;  // 右侧速度
            break;
        case PlayerState::Right2:
            m_velocity.x = m_velocity.y * (m_isAccelerating ? SPEED_SCALE : 1.f) * XY_SPEED_2;  // 右侧速度
            break;
        case PlayerState::Stop:
            m_velocity.x = 0.f;  // 停止移动
            break;
    }
}

void Player::updateYSpeed(float dt) {
    switch (m_state) {
        case PlayerState::Center:
        case PlayerState::Left1:
        case PlayerState::Left2:
        case PlayerState::Right1:
        case PlayerState::Right2:
            if (m_velocity.y < MAX_SPEED * (m_isAccelerating ? SPEED_SCALE : 1.f)) {
                m_velocity.y += dt * (m_isAccelerating ? ACCELERATION_2 : ACCELERATION_1);  // 增加速度
                if (m_velocity.y > MAX_SPEED * (m_isAccelerating ? SPEED_SCALE : 1.f)) {
                    m_velocity.y = MAX_SPEED * (m_isAccelerating ? SPEED_SCALE : 1.f);  // 限制最大速度
                }
            }
            break;
        case PlayerState::Stop:
            if (m_velocity.y > 0.f) {
                m_velocity.y -= ACCELERATION_3 * dt;  // 减少速度
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
        m_sprite.setTexture(*Utils::getTexture(paths[m_currentFrame]));
    }
}
