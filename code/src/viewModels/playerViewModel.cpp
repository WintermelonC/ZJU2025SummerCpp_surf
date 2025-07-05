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
    initializeAnimations();
    
    //  订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, 
                                std::shared_ptr<INotificationObserver>(this, [](INotificationObserver*){}));
}

void PlayerViewModel::update(const float deltaTime, const sf::Vector2f& mousePos) {
    m_playerModel.update(deltaTime, mousePos);
    m_animationViewModel.update(deltaTime);
    updatePlayerAnimation();
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

void PlayerViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            if (resetData.resetPlayerState) {
                resetPlayerState();
            }
            break;
        }
        default:
            break;
    }
}

void PlayerViewModel::resetPlayerState() {
    std::cout << "Resetting player state..." << std::endl;

    //  重置玩家模型
    m_playerModel.reset();
    
    //  重置动画到初始状态
    m_animationViewModel.play("center");
    
    //  重置精灵位置
    m_spriteViewModel->setSpritePosition(SpriteType::player, m_playerModel.getPosition());
}