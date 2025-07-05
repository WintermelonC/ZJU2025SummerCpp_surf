#include "gameViewModel.h"

GameViewModel::GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM),
      m_ObstacleItemViewModel(spriteVM),
      m_playerViewModel(spriteVM) {
    m_spriteViewModel->setSprite(
        SpriteType::water,
        sf::Color::White,
        {0, 0},
        {1, 1},
        false
    );
    
    //  订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, 
                                std::shared_ptr<INotificationObserver>(this, [](INotificationObserver*){}));
}

void GameViewModel::update(const sf::Vector2f& mousePos) {
    float deltaTime = m_clock.restart().asSeconds();
    m_playerViewModel.update(deltaTime, mousePos);
    m_ObstacleItemViewModel.update(deltaTime, m_playerViewModel.getPlayerVelocity(), !m_playerViewModel.isPlayerStop());
    updateWater();
}

void GameViewModel::usePower() {
    m_playerViewModel.usePower();
}

void GameViewModel::updateWater() {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= m_playerViewModel.getPlayerVelocity() * Config::Game::PARALLAX_FACTOR;
    
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
            
            //  重置游戏模型
            if (resetData.resetScore) {
                m_gameModel.reset();
            }
            
            //  重置水面位置
            m_waterOffset = {0, 0};
            m_spriteViewModel->setSpritePosition(SpriteType::water, m_waterOffset);
            
            //  重置时钟
            m_clock.restart();
            
            break;
        }
        default:
            break;
    }
}
