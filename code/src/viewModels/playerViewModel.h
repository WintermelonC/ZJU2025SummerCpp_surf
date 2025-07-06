#pragma once

#include <deque>
#include "../common/notificationCenter.h"
#include "../common/utils.h"
#include "../models/playerModel.h"
#include "../models/gameModel.h"
#include "../common/textureManager.h"

class PlayerViewModel : public INotificationObserver, public std::enable_shared_from_this<PlayerViewModel> {
public:
    PlayerViewModel(std::shared_ptr<TextureManager> textureVM);

    // 主要更新方法
    void update(const float deltaTime, const sf::Vector2f& mousePos, const sf::Vector2u& windowSize);
    void usePower() { m_playerModel.usePower(); }

    // 在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    // Getter 方法
    const sf::Vector2f& getPlayerVelocity() const { return m_playerModel.getVelocity(); }
    const Config::PlayerState& getPlayerState() const { return m_playerModel.getState(); }
    const sf::Vector2f& getPlayerPosition() const { return m_playerModel.getPosition(); }
    const bool isPlayerStop() const { return m_playerModel.getState() == Config::PlayerState::stop; }
    
    // 获取UI元素
    std::deque<Config::Trail>& getRipples() { return m_ripples; }
    std::deque<Config::Trail>& getTails() { return m_tails; }
    std::vector<sf::Sprite>& getHeartSprites() { return m_heartSprites; }
    std::vector<sf::Sprite>& getPowerSprites() { return m_powerSprites; }

    // 获取回调方法
    Config::PlayerUpdateCallback getUpdateCommand();
    Config::MouseRightClickCallback getMouseRightClickCommand();

    // Setter 方法
    void setGameState(const Config::GameState* gameState) { m_gameState = gameState; }

private:
    // 重置方法
    void resetPlayerState();  // 内部重置方法
    
    // 特效更新方法
    void updateRipple(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn = false);
    void updateTail(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn = false);
    void spawnRipple(const sf::Angle& angle, const bool& ifSpawn = false);
    void spawnTail(const sf::Angle& angle, const bool& ifSpawn = false);
    
    // UI更新方法
    void updateHeart(const sf::Vector2u& windowSize);
    void updatePower(const sf::Vector2u& windowSize);

private:
    // UI布局配置
    const int m_heartXOffset = 260;
    const int m_powerXOffset = 100;
    const float m_hpGap = 40.f;
    const float m_powerGap = m_hpGap;
    const sf::Vector2f m_hpScale = Config::Player::PLAYER_SCALE;
    const sf::Vector2f m_powerScale = m_hpScale;
    
    // 水波特效配置
    const int m_rippleCount = 4;  // 水波数量
    const float m_rippleLifetime = 0.4f;  // 水波存活时间
    const int m_rippleAlpha = 200;
    const sf::Color m_rippleColor = sf::Color(255, 255, 255, m_rippleAlpha); 
    
    // 拖尾特效配置
    const int m_tailCount = 1;  // 加速拖尾数量
    const float m_tailLifetime = 1.0f;  // 加速拖尾存活时间
    const int m_tailAlpha = 150;
    const sf::Color m_tailColor = sf::Color(141, 249, 196, m_tailAlpha);

    // 核心模型和视图模型
    PlayerModel m_playerModel;
    std::shared_ptr<TextureManager> m_textureManager;

    // UI元素
    std::vector<sf::Sprite> m_heartSprites;  // 玩家生命值图标
    std::vector<sf::Sprite> m_powerSprites;  // 玩家能量值

    // 特效元素
    std::deque<Config::Trail> m_ripples;  // 水波
    std::deque<Config::Trail> m_tails;    // 加速拖尾
    
    // 外部依赖
    const Config::GameState* m_gameState;
};