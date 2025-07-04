#pragma once

#include "../models/playerModel.h"
#include "spriteViewModel.h"
#include "animationViewModel.h"

class PlayerViewModel {
public:
    PlayerViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const float deltaTime, const sf::Vector2f& mousePos);
    void usePower() { m_playerModel.usePower(); }

    const sf::Vector2f& getPlayerVelocity() const { return m_playerModel.getVelocity(); }
    const bool isPlayerStop() const { return m_playerModel.getState() == PlayerState::stop; }

private:
    void initialize();
    void initializeAnimations();
    void updatePlayerAnimation();

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