#include "gameViewModel.h"

GameViewModel::GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM) {
    m_map.addEntity({
        EntityType::water,
        *(m_spriteViewModel->getTexture(TextureType::water)->get()),
        {0, 0},
        {0, 0},
        {1, 1}
    });
    m_map.addEntity({
        EntityType::player,
        *(m_spriteViewModel->getTexture(TextureType::player_center_1)->get()),
        m_playerModel.getPosition(),
        m_playerModel.getSize() / 2.f,
        {1.5, 1.5},
        m_playerModel.getCollisionBox()
    });

    initializeAnimations();
}

void GameViewModel::update(const sf::Vector2f& mousePos) {
    float deltaTime = m_clock.restart().asSeconds();
    m_playerModel.update(deltaTime, mousePos);
    m_animationViewModel.update(deltaTime);
    updatePlayerAnimation();
    updateWater();
}

void GameViewModel::updateWater() {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= m_playerModel.getVelocity() * m_parallaxFactor;
    
    if (m_waterOffset.x <= 0) {
        m_waterOffset.x += m_waterSize;
    } else if (m_waterOffset.x >= m_waterSize) {
        m_waterOffset.x -= m_waterSize;
    }
    
    if (m_waterOffset.y <= -m_waterSize) {
        m_waterOffset.y += m_waterSize;
    }

    m_map.setPosition(EntityType::water, m_waterOffset);
}

void GameViewModel::updatePlayerAnimation() {
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

    m_map.setTexture(EntityType::player, 
        m_spriteViewModel->getTexture(m_animationViewModel.getCurrentFrame()));
}

void GameViewModel::initializeAnimations() {
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