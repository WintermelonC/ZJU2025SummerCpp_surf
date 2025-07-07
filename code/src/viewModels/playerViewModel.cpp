#include "playerViewModel.h"

PlayerViewModel::PlayerViewModel(std::shared_ptr<TextureManager> textureVM) 
    : m_textureManager(textureVM) {}
void PlayerViewModel::subscribeToNotifications() {
    auto& notificationCenter = NotificationCenter::getInstance();
    // 订阅游戏重置通知
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
    // 订阅碰撞相关通知
    notificationCenter.subscribe(NotificationType::DamageCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::SlowCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::PowerCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::HealthCollision, shared_from_this());
}

void PlayerViewModel::update(const float deltaTime, const sf::Vector2f& mousePos, const sf::Vector2u& windowSize) {
    if (m_gameState && *m_gameState != Config::GameState::playing) {
        return; // 如果游戏状态不是正在进行，则不更新玩家
    }
    m_playerModel.update(deltaTime, mousePos);
    updateHeart(windowSize);
    updatePower(windowSize);
    updateRipple(deltaTime, m_playerModel.getVelocity(), m_playerModel.getAngle(), m_playerModel.isTurn() && m_playerModel.getVelocity().y > Config::Player::SPEED_THRESHOLD_1);
    updateTail(deltaTime, m_playerModel.getVelocity(), m_playerModel.getAngle(), m_playerModel.isPower() && m_playerModel.getVelocity().y > Config::Player::SPEED_THRESHOLD_2);
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
        case NotificationType::DamageCollision: {
            // std::cout << "PlayerViewModel: Received DamageCollision notification." << std::endl;
            // 只有在非无敌状态下才处理伤害碰撞
            if (!m_playerModel.isInvincible()) {
                m_playerModel.takeDamage(1);
            }
            break;
        }
        case NotificationType::SlowCollision: {
            // 处理减速效果 - 应用减速状态
            if(!m_playerModel.isInvincible()) {
                std::cout << "PlayerViewModel: Received SlowCollision notification." << std::endl;
                m_playerModel.applySlowEffect();
            }
            break;
        }
        case NotificationType::PowerCollision: {
            // 处理加速效果 - 增加玩家能量值
            m_playerModel.restorePower(1);
            break;
        }
        case NotificationType::HealthCollision: {
            // 处理生命恢复 - 增加玩家生命值
            m_playerModel.restoreHealth(1);
            break;
        }
        default:
            break;
    }
}

void PlayerViewModel::resetPlayerState() {
    // 重置玩家模型
    m_playerModel.reset();
    m_ripples.clear();  // 清空水波
    m_tails.clear();  // 清空拖尾
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

void PlayerViewModel::updateHeart(const sf::Vector2u& windowSize) {
    m_heartSprites.clear();
    int HP = m_playerModel.getHp();
    for (int i = 1; i <= Config::Player::PLAYER_HP; i++) {
        sf::Sprite heart = (i <= HP) ? m_textureManager->getNewSprite(TextureType::heart_1) : m_textureManager->getNewSprite(TextureType::heart_2);
        Utils::setSprite(
            heart,
            sf::Color::White,
            {Config::Window::RENDER_CENTER.x - m_heartXOffset + i * m_hpGap, 
             Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50},
            m_hpScale
        );
        m_heartSprites.push_back(heart);
    }
}

void PlayerViewModel::updatePower(const sf::Vector2u& windowSize) {
    m_powerSprites.clear();
    int powerCount = m_playerModel.getPower();
    for (int i = 1; i <= Config::Player::PLAYER_POWER; i++) {
        sf::Sprite power = (i <= powerCount) ? m_textureManager->getNewSprite(TextureType::power_1) : m_textureManager->getNewSprite(TextureType::power_2);
        Utils::setSprite(
            power,
            sf::Color::White,
            {Config::Window::RENDER_CENTER.x + m_powerXOffset + i * m_powerGap, 
             Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50},
            m_powerScale
        );
        m_powerSprites.push_back(power);
    }
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
        line.setPosition({Config::Player::PLAYER_POS.x + std::tan(angle.asRadians()) * Config::Player::PLAYER_SIZE.y / 3.f
                          + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
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
        line.setPosition({Config::Player::PLAYER_POS.x + std::tan(angle.asRadians()) * Config::Player::PLAYER_SIZE.y / 3.f
                          + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
                          Config::Player::PLAYER_POS.y});
        line.setFillColor(m_tailColor);

        m_tails.push_back({line, m_tailLifetime});
    }
}

void PlayerViewModel::updateWaiting() {
    m_playerModel.setWaiting(false);
    m_playerModel.setInvincible();
}