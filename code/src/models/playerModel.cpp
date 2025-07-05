#include "playerModel.h"

PlayerModel::PlayerModel()
    : EntityModel(EntityModelType::player, Config::Player::PLAYER_POS, Config::Player::PLAYER_SIZE) {
    setCollisionBox(
        position,
        size / 2.f,
        size,
        sf::degrees(0.0f)
    );
}

void PlayerModel::update(const float deltaTime, const sf::Vector2f& mousePos) {
    updateState(mousePos);
    updateAngle();
    updatePower(deltaTime);  // 更新能量状态
    updateYSpeed(deltaTime);
    updateXSpeed(deltaTime);
}

void PlayerModel::usePower() {
    if (m_power > 0) {
        m_power--;
        m_powerTimer = 0.0f;
        m_isPower = true;
    }
}

void PlayerModel::updateState(const sf::Vector2f& mousePos) {
    const sf::Vector2f delta = mousePos - position;
    const sf::Angle angle = sf::radians(std::atan2(delta.x, delta.y));
    
    if (delta.y >= 0.0f) {
        const float angleDeg = angle.asDegrees();
        if (angleDeg >= m_angle2 || angleDeg <= -m_angle2) {
            m_state = (angleDeg >= 0.f) ? PlayerState::right2 : PlayerState::left2;
        } else if (angleDeg >= m_angle1 || angleDeg <= -m_angle1) {
            m_state = (angleDeg >= 0.f) ? PlayerState::right1 : PlayerState::left1;
        } else {
            m_state = PlayerState::center;
        }
    } else {
        m_state = PlayerState::stop;
    }
}

void PlayerModel::updateAngle() {
    switch (m_state) {
        case PlayerState::left2:
            m_angle = sf::degrees(-m_angle2);
            break;
        case PlayerState::left1:
            m_angle = sf::degrees(-m_angle1);
            break;
        case PlayerState::center:
            m_angle = sf::degrees(0.0f);
            break;
        case PlayerState::right1:
            m_angle = sf::degrees(m_angle1);
            break;
        case PlayerState::right2:
            m_angle = sf::degrees(m_angle2);
            break;
        case PlayerState::stop:
            m_angle = sf::degrees(0.0f);
            break;
    }
}

void PlayerModel::updatePower(const float& dt) {
    if (m_isPower) {
        m_powerTimer += dt;
        if (m_powerTimer >= m_powerTime) {
            m_isPower = false;  // 能量时间结束
            m_powerTimer = 0.0f;  // 重置计时器
        }
    } else {
        m_powerTimer = 0.0f;  // 重置能量计时器
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
            acceleration = m_acceleration2;
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

void PlayerModel::reset() {
    //  重置玩家状态
    m_velocity = {0, 0};
    m_angle = sf::degrees(0.0f);
    m_state = PlayerState::center;
    m_isPower = false;
    m_powerTimer = 0.0f;
    m_power = Config::Player::PLAYER_POWER;
    m_hp = Config::Player::PLAYER_HP;
    
    //  重置位置到初始位置
    position = Config::Player::PLAYER_POS;
    
    //  重置碰撞盒
    setCollisionBox(
        position,
        size / 2.f,
        size,
        sf::degrees(0.0f)
    );
}
