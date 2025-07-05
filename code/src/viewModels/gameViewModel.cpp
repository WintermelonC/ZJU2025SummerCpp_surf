#include "gameViewModel.h"

GameViewModel::GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM) {
    m_spriteViewModel->setSprite(
        SpriteType::water,
        sf::Color::White,
        {0, 0},
        {1, 1},
        false
    );
}

void GameViewModel::subscribeToNotifications() {
    // 订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
}

void GameViewModel::update(const sf::Vector2u& windowSize) {
    if (m_gameModel.getGameState() != Config::GameState::playing) {
        return;
    }
    m_spriteViewModel->setSprite(
        SpriteType::scoreboard,
        m_buttonColor,
        {Config::Window::RENDER_CENTER.x,
         Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50}
    );

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

    m_spriteViewModel->setSpritePosition(SpriteType::water, m_waterOffset);
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
            m_spriteViewModel->setSpritePosition(SpriteType::water, m_waterOffset);
            
            break;
        }
        default:
            break;
    }
}
