#include "viewModels/PlayerViewModel.h"
#include "views/spriteManager.h"
#include "common/config.h"
#include <algorithm>
#include <cmath>

PlayerViewModel::PlayerViewModel(std::shared_ptr<PlayerModel> playerModel)
    : m_playerModel(playerModel),
      m_powerTimer(0.0f),
      m_animationTimer(0.0f),
      m_wasInWater(false) {
}

void PlayerViewModel::update(float deltaTime, const sf::Vector2f& mousePos) {
    updateState(mousePos);
    updateTurn();
    updatePower(deltaTime);
    updateYSpeed(deltaTime);
    updateXSpeed();
    updateAnimation(deltaTime);
    
    // 更新玩家模型（包括动画）
    m_playerModel->update(deltaTime);
}

void PlayerViewModel::usePower() {
    m_playerModel->usePower();
    m_powerTimer = 0.0f;
}

void PlayerViewModel::takeDamage() {
    m_playerModel->takeDamage();
}

void PlayerViewModel::reset() {
    m_playerModel->resetPlayer();
    m_powerTimer = 0.0f;
    m_animationTimer = 0.0f;
    m_wasInWater = false;
}

void PlayerViewModel::updateState(const sf::Vector2f& mousePos) {
    const sf::Vector2f delta = mousePos - m_playerModel->getPosition();
    const sf::Angle angle = sf::radians(std::atan2(delta.x, delta.y));
    
    if (delta.y >= 0.0f) {
        const float angleDeg = angle.asDegrees();
        if (angleDeg >= ANGLE_2 || angleDeg <= -ANGLE_2) {
            m_playerModel->setPlayerState((angleDeg >= 0.f) ? PlayerState::Right2 : PlayerState::Left2);
            m_playerModel->setAngle(sf::degrees((angleDeg >= 0.f) ? ANGLE_2 : -ANGLE_2));
        } else if (angleDeg >= ANGLE_1 || angleDeg <= -ANGLE_1) {
            m_playerModel->setPlayerState((angleDeg >= 0.f) ? PlayerState::Right1 : PlayerState::Left1);
            m_playerModel->setAngle(sf::degrees((angleDeg >= 0.f) ? ANGLE_1 : -ANGLE_1));
        } else {
            m_playerModel->setPlayerState(PlayerState::Center);
            m_playerModel->setAngle(sf::degrees(0.0f));
        }
    } else {
        m_playerModel->setPlayerState(PlayerState::Stop);
        m_playerModel->setAngle(sf::degrees(0.0f));
    }
}

void PlayerViewModel::updateTurn() {
    static PlayerState lastState = PlayerState::Center;
    const PlayerState currentState = m_playerModel->getPlayerState();
    
    // 检测转弯
    bool isTurn = (lastState != currentState) && 
                  (currentState == PlayerState::Left1 || currentState == PlayerState::Left2 ||
                   currentState == PlayerState::Right1 || currentState == PlayerState::Right2);
    
    m_playerModel->setTurn(isTurn);
    lastState = currentState;
}

void PlayerViewModel::updatePower(float deltaTime) {
    if (m_playerModel->isPower()) {
        m_powerTimer += deltaTime;
        if (m_powerTimer >= Config::Player::POWER_DURATION) {
            m_playerModel->setPower(false);
            m_powerTimer = 0.0f;
        }
    }
}

void PlayerViewModel::updateYSpeed(float deltaTime) {
    sf::Vector2f velocity = m_playerModel->getVelocity();
    
    if (m_playerModel->getPlayerState() == PlayerState::Stop) {
        // 减速
        velocity.y = std::max(0.0f, velocity.y - ACCELERATION_2 * deltaTime);
    } else {
        // 加速
        float acceleration = ACCELERATION_1;
        float maxSpeed = MAX_SPEED;
        if (m_playerModel->isPower()) {
            acceleration = ACCELERATION_2;
            maxSpeed = MAX_SPEED * Config::Player::POWER_MULTIPLIER;
        }
        
        velocity.y = std::min(maxSpeed, velocity.y + acceleration * deltaTime);
    }
    
    m_playerModel->setVelocity(velocity);
}

void PlayerViewModel::updateXSpeed() {
    sf::Vector2f velocity = m_playerModel->getVelocity();
    
    switch (m_playerModel->getPlayerState()) {
        case PlayerState::Center:
            velocity.x = 0.0f;
            break;
        case PlayerState::Left1:
            velocity.x = -velocity.y * XY_SPEED_1;
            break;
        case PlayerState::Left2:
            velocity.x = -velocity.y * XY_SPEED_2;
            break;
        case PlayerState::Right1:
            velocity.x = velocity.y * XY_SPEED_1;
            break;
        case PlayerState::Right2:
            velocity.x = velocity.y * XY_SPEED_2;
            break;
        case PlayerState::Stop:
            velocity.x = 0.0f;
            break;
    }
    
    m_playerModel->setVelocity(velocity);
}

void PlayerViewModel::updateAnimation(float deltaTime) {
    SpriteManager::setSprite(SpriteType::player, Config::Player::PLAYER_POS, Config::Player::PLAYER_SCALE, true);
    
    // 使用玩家模型的当前动画帧
    Textures currentFrame = m_playerModel->getCurrentAnimationFrame();
    SpriteManager::setSpriteTexture(SpriteType::player, currentFrame);
}
