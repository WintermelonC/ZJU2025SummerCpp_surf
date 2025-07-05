#pragma once

#include <deque>
#include "../models/playerModel.h"
#include "../models/gameModel.h"
#include "spriteViewModel.h"
#include "animationViewModel.h"
#include "../common/notificationCenter.h"
#include "../common/utils.h"

class PlayerViewModel : public INotificationObserver, public std::enable_shared_from_this<PlayerViewModel> {
public:
    PlayerViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const float deltaTime, const sf::Vector2f& mousePos);
    void usePower() { m_playerModel.usePower(); }

    //  在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    const sf::Vector2f& getPlayerVelocity() const { return m_playerModel.getVelocity(); }
    const PlayerState& getPlayerState() const { return m_playerModel.getState(); }
    std::function<void(const float&, const sf::Vector2f&)> getUpdateCommand();
    const bool isPlayerStop() const { return m_playerModel.getState() == PlayerState::stop; }

    std::function<void()> getMouseRightClickCommand();
    std::deque<Config::Trail>& getRipples() { return m_ripples; }
    std::deque<Config::Trail>& getTails() { return m_tails; }

    void setGameState(const Config::GameState* gameState) { m_gameState = gameState; }

private:
    void initializeAnimations();
    void updatePlayerAnimation();
    void resetPlayerState(); // 内部重置方法
    void updateRipple(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn = false);
    void updateTail(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn = false);
    void spawnRipple(const sf::Angle& angle, const bool& ifSpawn = false);
    void spawnTail(const sf::Angle& angle, const bool& ifSpawn = false);

private:
    const int m_heartXOffset = 260;
    const int m_powerXOffset = 100;
    const float m_hpGap = 40.f;
    const float m_powerGap = m_hpGap;
    const sf::Vector2f m_hpScale = Config::Player::PLAYER_SCALE;
    const sf::Vector2f m_powerScale = m_hpScale;
    const Config::GameState* m_gameState;
    const int m_rippleCount = 4;  // 水波数量
    const float m_rippleLifetime = 0.4f;  // 水波存活时间
    const int m_rippleAlpha = 200;
    const sf::Color m_rippleColor = sf::Color(255, 255, 255, m_rippleAlpha); 
    const int m_tailCount = 1;  // 加速拖尾数量
    const float m_tailLifetime = 1.0f;  // 加速拖尾存活时间
    const int m_tailAlpha = 150;
    const sf::Color m_tailColor = sf::Color(141, 249, 196, m_tailAlpha);

    PlayerModel m_playerModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    AnimationViewModel m_animationViewModel;

    std::vector<sf::Sprite> m_heartSprites;  // 玩家生命值图标
    std::vector<sf::Sprite> m_powerSprites;  // 玩家能量值

    std::deque<Config::Trail> m_ripples;  // 水波
    std::deque<Config::Trail> m_tails;  // 加速拖尾
};