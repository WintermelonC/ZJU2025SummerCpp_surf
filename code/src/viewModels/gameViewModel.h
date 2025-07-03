#pragma once

#include <SFML/Graphics.hpp>
#include "../models/playerModel.h"
#include "animationViewModel.h"
#include "spriteViewModel.h"
#include "ObstacleItemViewModel.h"

class GameViewModel {
public:
    GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const sf::Vector2f& mousePos);

private:
    void updateWater();
    void updatePlayerAnimation();

    void initializeAnimations();

private:
    const float m_parallaxFactor = 0.1f; // 视差因子
    const float m_waterSize = 256.f;

    PlayerModel m_playerModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    sf::Clock m_clock;
    sf::Vector2f m_waterOffset = {0, 0};
    sf::Clock m_playerClock;
    AnimationViewModel m_animationViewModel;
    ObstacleItemViewModel m_ObstacleItemViewModel;
};