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
    void initializeAnimations();
    void updatePlayerAnimation();

private:
    PlayerModel m_playerModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    AnimationViewModel m_animationViewModel;
};