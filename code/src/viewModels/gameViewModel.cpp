#include "gameViewModel.h"

GameViewModel::GameViewModel() {}
void GameViewModel::subscribeToNotifications() {
    auto& notificationCenter = NotificationCenter::getInstance();
    // 订阅游戏重置通知
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
    // 订阅碰撞通知以更新游戏状态
    notificationCenter.subscribe(NotificationType::DamageCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::PowerCollision, shared_from_this());
    notificationCenter.subscribe(NotificationType::HealthCollision, shared_from_this());
}

void GameViewModel::update(const sf::Vector2u& windowSize) {
    if (m_gameModel.getGameState() != Config::GameState::playing) {
        return;
    }
    m_gameModel.update(*m_playerVelocity);
    updateWater(*m_playerVelocity);
}

void GameViewModel::updateWater(const sf::Vector2f& playerVelocity) {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= playerVelocity * Config::Game::PARALLAX_FACTOR;
    
    if (m_waterOffset.x <= 0) {
        m_waterOffset.x += m_waterSize;
    } else if (m_waterOffset.x >= m_waterSize) {
        m_waterOffset.x -= m_waterSize;
    }
    
    if (m_waterOffset.y <= -m_waterSize) {
        m_waterOffset.y += m_waterSize;
    }
}

void GameViewModel::resetGame() {
    //  发送游戏重置通知
    NotificationCenter::getInstance().postGameReset(true, true, true);
}

void GameViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            
            // 重置游戏模型
            if (resetData.resetScore) {
                m_gameModel.reset();
            }
            
            // 重置水面位置
            m_waterOffset = {0, 0};
            break;
        }
        default:
            break;
    }
}
