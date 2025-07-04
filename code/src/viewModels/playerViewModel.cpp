#include "playerViewModel.h"

PlayerViewModel::PlayerViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM) {
    // 设置玩家精灵
    m_spriteViewModel->setSprite(
        SpriteType::player,
        sf::Color::White,
        m_playerModel.getPosition(),
        Config::Player::PLAYER_SCALE
    );
    initialize();
}

void PlayerViewModel::update(const float deltaTime, const sf::Vector2f& mousePos) {
    m_playerModel.update(deltaTime, mousePos);
    m_animationViewModel.update(deltaTime);
    updatePlayerAnimation();
}

void PlayerViewModel::initialize() {
    initializeAnimations();
    const int heartCount = m_playerModel.getHp();
    const int powerCount = m_playerModel.getPower();
    
    for (int i = 1; i <= Config::Player::PLAYER_HP; i++) {
        TextureType textureType = (i <= heartCount) ? TextureType::heart_1 : TextureType::heart_2;
        SpriteType SpriteType = (i <= heartCount) ? SpriteType::heart_1 : SpriteType::heart_2;
        sf::Sprite heartSprite = m_spriteViewModel->getNewSprite(SpriteType, textureType);
        m_spriteViewModel->setSprite(
            heartSprite,
            sf::Color::White,
            {Config::Window::RENDER_CENTER.x - m_heartXOffset + i * HP_GAP, 
             Config::Window::RENDER_CENTER.y - window.getSize().y / 2 + 50},
            {HP_SCALE, HP_SCALE},
        );
    }
}

void PlayerViewModel::updatePlayerAnimation() {
    // 更新玩家动画
    std::string targetAnimation;
    
    switch (m_playerModel.getState()) {
        case PlayerState::center:
            targetAnimation = "center";
            break;
        case PlayerState::left1:
            targetAnimation = "left1";
            break;
        case PlayerState::left2:
            targetAnimation = "left2";
            break;
        case PlayerState::right1:
            targetAnimation = "right1";
            break;
        case PlayerState::right2:
            targetAnimation = "right2";
            break;
        case PlayerState::stop:
            targetAnimation = "stop";
            break;
    }
    
    // 如果需要切换动画，则切换
    if (m_animationViewModel.getCurrentAnimationName() != targetAnimation) {
        m_animationViewModel.play(targetAnimation);
    }

    m_spriteViewModel->setSpriteTexture(SpriteType::player, m_animationViewModel.getCurrentFrame());
}

void PlayerViewModel::initializeAnimations() {
    // 初始化动画
    m_animationViewModel.addAnimation("center", AnimationConfig({
        TextureType::player_center_1,
        TextureType::player_center_2,
        TextureType::player_center_3
    }, 0.1f, true));

    m_animationViewModel.addAnimation("left1", AnimationConfig({
        TextureType::player_left_11,
        TextureType::player_left_12,
        TextureType::player_left_13
    }, 0.1f, true));

    m_animationViewModel.addAnimation("left2", AnimationConfig({
        TextureType::player_left_21,
        TextureType::player_left_22,
        TextureType::player_left_23
    }, 0.1f, true));

    m_animationViewModel.addAnimation("right1", AnimationConfig({
        TextureType::player_right_11,
        TextureType::player_right_12,
        TextureType::player_right_13
    }, 0.1f, true));

    m_animationViewModel.addAnimation("right2", AnimationConfig({
        TextureType::player_right_21,
        TextureType::player_right_22,
        TextureType::player_right_23
    }, 0.1f, true));

    m_animationViewModel.addAnimation("stop", AnimationConfig({
        TextureType::player_stop_1,
        TextureType::player_stop_2,
        TextureType::player_stop_3
    }, 0.1f, true));

    m_animationViewModel.play("center");
}