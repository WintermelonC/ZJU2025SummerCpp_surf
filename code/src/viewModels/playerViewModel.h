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
    
    // 🔔 实现观察者接口
    void onNotification(const NotificationData& data) override;

    const sf::Vector2f& getPlayerVelocity() const { return m_playerModel.getVelocity(); }
    const bool isPlayerStop() const { return m_playerModel.getState() == PlayerState::stop; }

private:
    void initializeAnimations();
    void updatePlayerAnimation();
    void resetPlayerState(); // 🔔 内部重置方法

private:
    PlayerModel m_playerModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    AnimationViewModel m_animationViewModel;
};