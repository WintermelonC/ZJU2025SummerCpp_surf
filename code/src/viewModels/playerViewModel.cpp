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
}

void PlayerViewModel::subscribeToNotifications() {
    //  订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
}

void PlayerViewModel::update(const float deltaTime, const sf::Vector2f& mousePos) {
    if (m_gameState && *m_gameState != Config::GameState::playing) {
        return; // 如果游戏状态不是正在进行，则不更新玩家
    }
    m_playerModel.update(deltaTime, mousePos);
    m_animationViewModel.update(deltaTime);
    updatePlayerAnimation();
    updateRipple(deltaTime, m_playerModel.getVelocity(), m_playerModel.getAngle(), m_playerModel.isTurn() && m_playerModel.getVelocity().y > Config::Player::SPEED_THRESHOLD_1);
    updateTail(deltaTime, m_playerModel.getVelocity(), m_playerModel.getAngle(), m_playerModel.isPower() && m_playerModel.getVelocity().y > Config::Player::SPEED_THRESHOLD_2);
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
    // 重置玩家模型
    m_playerModel.reset();
    
    // 重置动画到初始状态
    m_animationViewModel.play("center");
    
    // 重置精灵位置
    m_spriteViewModel->setSpritePosition(SpriteType::player, m_playerModel.getPosition());
}

void PlayerViewModel::updateRipple(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn) {
    for (auto it = m_ripples.begin(); it != m_ripples.end(); ) {
        it -> lifetime -= dt;
        if (it -> lifetime <= 0) {
            it = m_ripples.erase(it);  // 移除过期的水波
        } else {
            it -> trail.move({-velocity.x * Config::Game::PARALLAX_FACTOR, -velocity.y * Config::Game::PARALLAX_FACTOR});  // 更新水波位置
            it -> trail.setSize({3.f, it -> trail.getSize().y + dt * 100.f});
            sf::Color color = m_rippleColor;
            color.a = static_cast<int>(m_rippleAlpha * (it -> lifetime / m_rippleLifetime));  // 渐变透明度
            it -> trail.setFillColor(color);  // 渐变透明度
            it++;
        }
    }

    spawnRipple(-angle, ifSpawn);  // 生成新的水波
}

void PlayerViewModel::updateTail(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn) {
    for (auto it = m_tails.begin(); it != m_tails.end(); ) {
        it -> lifetime -= dt;
        if (it -> lifetime <= 0) {
            it = m_tails.erase(it);  // 移除过期的拖尾
        } else {
            it -> trail.move({-velocity.x * Config::Game::PARALLAX_FACTOR, -velocity.y * Config::Game::PARALLAX_FACTOR});  // 更新拖尾位置
            sf::Color color = m_tailColor;
            color.a = static_cast<int>(m_tailAlpha * (it -> lifetime / m_tailLifetime));  // 渐变透明度
            it -> trail.setFillColor(color);  // 渐变透明度
            it++;
        }
    }

    spawnTail(-angle, ifSpawn);  // 生成新的拖尾
}

void PlayerViewModel::spawnRipple(const sf::Angle& angle, const bool& ifSpawn) {
    if (!ifSpawn) {
        return;
    }

    // 产生 m_rippleCount 个线段
    for (int i = 0; i < m_rippleCount; i++) {
        sf::RectangleShape line({3.f, 
                                 Utils::randomFloat(5.f, 10.f)});
        line.setRotation(angle + sf::degrees(Utils::randomFloat(-5.f, 5.f)));
        line.setPosition({Config::Player::PLAYER_POS.x + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
                          Config::Player::PLAYER_POS.y});
        line.setFillColor(m_rippleColor);

        m_ripples.push_back({line, m_rippleLifetime});
    }
}

void PlayerViewModel::spawnTail(const sf::Angle& angle, const bool& ifSpawn) {
    if (!ifSpawn) {
        return;
    }

    // 产生 m_tailCount 个线段
    for (int i = 0; i < m_tailCount; i++) {
        sf::RectangleShape line({3.f, 
                                 Utils::randomFloat(50.f, 60.f)});
        line.setRotation(angle + sf::degrees(Utils::randomFloat(-5.f, 5.f)));
        line.setOrigin({line.getSize().x / 2.f, line.getSize().y});
        line.setPosition({Config::Player::PLAYER_POS.x + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
                          Config::Player::PLAYER_POS.y});
        line.setFillColor(m_tailColor);

        m_tails.push_back({line, m_tailLifetime});
    }
}
