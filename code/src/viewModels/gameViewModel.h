#pragma once

#include <SFML/Graphics.hpp>
#include "spriteViewModel.h"
#include "../models/gameModel.h"
#include "ObstacleItemViewModel.h"
#include "playerViewModel.h"

class GameViewModel {
public:
    GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const sf::Vector2f& mousePos, const sf::Vector2u& windowSize, const sf::Vector2f& playerVelocity);

    const float* getScore() const { return &m_gameModel.getScore(); }
    const float getDeltaTime() const { return m_clock.getElapsedTime().asSeconds(); }

private:
    void updateWater(const sf::Vector2f& playerVelocity);

private:
    const float m_waterSize = 256.f;
    const sf::Color m_buttonColor = sf::Color(195, 240, 247);
    
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    sf::Clock m_clock;
    sf::Vector2f m_waterOffset = {0, 0};
    GameModel m_gameModel;
};