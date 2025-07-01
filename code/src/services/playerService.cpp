#include "services/playerService.h"
#include "views/entityManager.h"
#include "infrastructure/config.h"
#include <algorithm>
#include <cmath>

PlayerService::PlayerService(std::shared_ptr<PlayerModel> playerModel)
    : m_playerModel(playerModel),
      m_powerTimer(0.0f),
      m_animationTimer(0.0f),
      m_wasInWater(false) {
}

void PlayerService::update(float deltaTime, const sf::Vector2f& mousePos) {
    updateState(mousePos);
    updateTurn();
    updatePower(deltaTime);
    updateYSpeed(deltaTime);
    updateXSpeed();
    updateAnimation(deltaTime);
    updatePosition(deltaTime);
}

void PlayerService::usePower() {
    m_playerModel->usePower();
    m_powerTimer = 0.0f;
}

void PlayerService::takeDamage() {
    m_playerModel->takeDamage();
}

void PlayerService::reset() {
    m_playerModel->resetPlayer();
    m_powerTimer = 0.0f;
    m_animationTimer = 0.0f;
    m_wasInWater = false;
}

void PlayerService::updateState(const sf::Vector2f& mousePos) {
    const sf::Vector2f delta = mousePos - m_playerModel->getPosition();
    const sf::Angle angle = sf::radians(std::atan2(delta.x, delta.y));
    
    m_playerModel->setAngle(angle);
    
    if (delta.y >= 0.0f) {
        const float angleDeg = angle.asDegrees();
        if (angleDeg >= ANGLE_2 || angleDeg <= -ANGLE_2) {
            m_playerModel->setState((angleDeg >= 0.f) ? PlayerState::Right2 : PlayerState::Left2);
        } else if (angleDeg >= ANGLE_1 || angleDeg <= -ANGLE_1) {
            m_playerModel->setState((angleDeg >= 0.f) ? PlayerState::Right1 : PlayerState::Left1);
        } else {
            m_playerModel->setState(PlayerState::Center);
        }
    } else {
        m_playerModel->setState(PlayerState::Stop);
    }
}

void PlayerService::updateTurn() {
    static PlayerState lastState = PlayerState::Center;
    const PlayerState currentState = m_playerModel->getState();
    
    // 检测转弯
    bool isTurn = (lastState != currentState) && 
                  (currentState == PlayerState::Left1 || currentState == PlayerState::Left2 ||
                   currentState == PlayerState::Right1 || currentState == PlayerState::Right2);
    
    m_playerModel->setTurn(isTurn);
    lastState = currentState;
}

void PlayerService::updatePower(float deltaTime) {
    if (m_playerModel->isPower()) {
        m_powerTimer += deltaTime;
        if (m_powerTimer >= Config::Player::POWER_DURATION) {
            m_playerModel->setPower(false);
            m_powerTimer = 0.0f;
        }
    }
}

void PlayerService::updateYSpeed(float deltaTime) {
    sf::Vector2f velocity = m_playerModel->getVelocity();
    
    if (m_playerModel->getState() == PlayerState::Stop) {
        // 减速
        velocity.y = std::max(0.0f, velocity.y - DECELERATION * deltaTime);
    } else {
        // 加速
        float acceleration = ACCELERATION;
        if (m_playerModel->isPower()) {
            acceleration *= Config::Player::POWER_MULTIPLIER;
        }
        
        velocity.y = std::min(MAX_SPEED, velocity.y + acceleration * deltaTime);
    }
    
    m_playerModel->setVelocity(velocity);
}

void PlayerService::updateXSpeed() {
    sf::Vector2f velocity = m_playerModel->getVelocity();
    
    switch (m_playerModel->getState()) {
        case PlayerState::Center:
            velocity.x = 0.0f;
            break;
        case PlayerState::Left1:
            velocity.x = -Config::Player::TURN_SPEED_1;
            break;
        case PlayerState::Left2:
            velocity.x = -Config::Player::TURN_SPEED_2;
            break;
        case PlayerState::Right1:
            velocity.x = Config::Player::TURN_SPEED_1;
            break;
        case PlayerState::Right2:
            velocity.x = Config::Player::TURN_SPEED_2;
            break;
        case PlayerState::Stop:
            velocity.x *= 0.95f; // 逐渐减速
            break;
    }
    
    m_playerModel->setVelocity(velocity);
}

void PlayerService::updateAnimation(float deltaTime) {
    m_animationTimer += deltaTime;
    
    // 根据状态和时间更新动画帧
    // 这里可以实现动画逻辑
    EntityManager::setSprite(
        EntityType::player, 
        m_playerModel->getPosition(), 
        {1.0f, 1.0f}, // 缩放
        false, // 不水平翻转
        {0.0f, 0.0f} // 偏移
    );
}

void PlayerService::updatePosition(float deltaTime) {
    sf::Vector2f position = m_playerModel->getPosition();
    const sf::Vector2f velocity = m_playerModel->getVelocity();
    
    // 更新位置（注意：在这个游戏中玩家位置相对固定，主要是背景移动）
    // 但我们仍然可以记录逻辑位置用于碰撞检测等
    position += velocity * deltaTime;
    
    m_playerModel->setPosition(position);
}
