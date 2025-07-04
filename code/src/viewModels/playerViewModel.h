#pragma once

#include "../models/playerModel.h"
#include "spriteViewModel.h"
#include "animationViewModel.h"
#include "../common/notificationCenter.h"

class PlayerViewModel : public INotificationObserver {
public:
    PlayerViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const float deltaTime, const sf::Vector2f& mousePos);
    void usePower() { m_playerModel.usePower(); }
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    const sf::Vector2f& getPlayerVelocity() const { return m_playerModel.getVelocity(); }
    const bool isPlayerStop() const { return m_playerModel.getState() == PlayerState::stop; }

    std::function<void()> getMouseRightClickCommand();

private:
    void initializeAnimations();
    void updatePlayerAnimation();
    void resetPlayerState(); // 内部重置方法

private:
    const int m_heartXOffset = 260;
    const int m_powerXOffset = 100;
    const float m_hpGap = 40.f;
    const float m_powerGap = m_hpGap;
    const sf::Vector2f m_hpScale = Config::Player::PLAYER_SCALE;
    const sf::Vector2f m_powerScale = m_hpScale;

    PlayerModel m_playerModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    AnimationViewModel m_animationViewModel;

    std::vector<sf::Sprite> m_heartSprites;  // 玩家生命值图标
    std::vector<sf::Sprite> m_powerSprites;  // 玩家能量值
};