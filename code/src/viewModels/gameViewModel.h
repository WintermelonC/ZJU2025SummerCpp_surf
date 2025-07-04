#pragma once

#include <SFML/Graphics.hpp>
#include "animationViewModel.h"
#include "spriteViewModel.h"
#include "ObstacleItemViewModel.h"
#include "playerViewModel.h"

class GameViewModel {
public:
    GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const sf::Vector2f& mousePos);
    void handleMouseEvents(const sf::Event::MouseButtonPressed& mouseButton);

    std::vector<sf::Sprite>& getObstacleItemSprites() { return m_ObstacleItemViewModel.getSprites(); }

private:
    void updateWater();

private:
    const float m_waterSize = 256.f;
    
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    sf::Clock m_clock;
    sf::Vector2f m_waterOffset = {0, 0};
    ObstacleItemViewModel m_ObstacleItemViewModel;
    PlayerViewModel m_playerViewModel;
};