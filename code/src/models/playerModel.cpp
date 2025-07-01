#include "models/playerModel.h"
#include <iostream>

PlayerModel::PlayerModel() 
    : EntityModel(EntityType::Player),
      m_playerState(PlayerState::Center),
      m_hp(Config::Player::PLAYER_HP),
      m_power(Config::Player::PLAYER_POWER),
      m_isTurn(false),
      m_isPower(false) {
    
    // 设置玩家特有的初始属性
    setPosition(Config::Player::PLAYER_POS);
    setVelocity({0.0f, 0.0f});
    setAngle(sf::degrees(0.0f));
    setSize({Config::Player::PLAYER_SIZE.x, Config::Player::PLAYER_SIZE.y});
    
    // 初始化动画
    initializeAnimations();
}

void PlayerModel::update(float deltaTime) {
    // 更新动画
    m_animationController.update(deltaTime);
    
    // 根据玩家状态更新动画
    updateAnimation();
}

sf::FloatRect PlayerModel::getCollisionBounds() const {
    // 玩家可能需要更精确的碰撞边界
    // 这里可以设置比视觉边界稍小的碰撞边界
    const float collisionScale = 0.8f; // 碰撞边界为视觉大小的80%
    sf::Vector2f collisionSize = getSize() * collisionScale;
    
    return sf::FloatRect({getPosition().x - collisionSize.x / 2.0f, getPosition().y - collisionSize.y / 2.0f}, collisionSize);
}

void PlayerModel::usePower() {
    if (m_power > 0) {
        m_power--;
        m_isPower = true;
        if (m_onPowerChanged) {
            m_onPowerChanged(m_power);
        }
    }
}

void PlayerModel::takeDamage() {
    if (m_hp > 0) {
        m_hp--;
        if (m_onHPChanged) {
            m_onHPChanged(m_hp);
        }
    }
}

void PlayerModel::resetPlayer() {
    setPosition(Config::Player::PLAYER_POS);
    setVelocity({0.0f, 0.0f});
    setAngle(sf::degrees(0.0f));
    m_playerState = PlayerState::Center;
    m_hp = Config::Player::PLAYER_HP;
    m_power = Config::Player::PLAYER_POWER;
    m_isTurn = false;
    m_isPower = false;
    setState(EntityState::Active);
}

Textures PlayerModel::getCurrentAnimationFrame() const {
    return m_animationController.getCurrentFrame();
}

void PlayerModel::initializeAnimations() {
    // 初始化各种状态的动画
    m_animationController.addAnimation("center", 
        AnimationConfig({Textures::player_center_1, Textures::player_center_2, Textures::player_center_3}, 0.1f, true));
    
    m_animationController.addAnimation("left1", 
        AnimationConfig({Textures::player_left_11, Textures::player_left_12, Textures::player_left_13}, 0.1f, true));
    
    m_animationController.addAnimation("left2", 
        AnimationConfig({Textures::player_left_21, Textures::player_left_22, Textures::player_left_23}, 0.1f, true));
    
    m_animationController.addAnimation("right1", 
        AnimationConfig({Textures::player_right_11, Textures::player_right_12, Textures::player_right_13}, 0.1f, true));
    
    m_animationController.addAnimation("right2", 
        AnimationConfig({Textures::player_right_21, Textures::player_right_22, Textures::player_right_23}, 0.1f, true));
    
    m_animationController.addAnimation("stop", 
        AnimationConfig({Textures::player_stop_1, Textures::player_stop_2, Textures::player_stop_3}, 0.1f, true));
    
    // 开始播放默认动画
    m_animationController.play("center");
}

void PlayerModel::updateAnimation() {
    std::string targetAnimation;
    
    switch (m_playerState) {
        case PlayerState::Center:
            targetAnimation = "center";
            break;
        case PlayerState::Left1:
            targetAnimation = "left1";
            break;
        case PlayerState::Left2:
            targetAnimation = "left2";
            break;
        case PlayerState::Right1:
            targetAnimation = "right1";
            break;
        case PlayerState::Right2:
            targetAnimation = "right2";
            break;
        case PlayerState::Stop:
            targetAnimation = "stop";
            break;
    }
    
    // 如果需要切换动画，则切换
    if (m_animationController.getCurrentAnimationName() != targetAnimation) {
        m_animationController.play(targetAnimation);
    }
    
// #ifdef DEBUG
//     // 调试信息：输出当前动画状态
//     static float debugTimer = 0.0f;
//     static float lastTime = 0.0f;
//     debugTimer += 0.016f; // 假设60FPS
//     if (debugTimer - lastTime >= 1.0f) { // 每秒输出一次
//         std::cout << "Player State: " << static_cast<int>(m_playerState) 
//                   << ", Animation: " << targetAnimation 
//                   << ", Current Frame: " << static_cast<int>(getCurrentAnimationFrame()) << std::endl;
//         lastTime = debugTimer;
//     }
// #endif
}
