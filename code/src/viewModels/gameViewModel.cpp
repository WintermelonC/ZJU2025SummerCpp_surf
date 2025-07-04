#include "gameViewModel.h"

GameViewModel::GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM) {
    m_spriteViewModel->setSprite(
        SpriteType::water,
        sf::Color::White,
        {0, 0},
        {1, 1},
        false
    );
}

void GameViewModel::update(const sf::Vector2f& mousePos, const sf::Vector2u& windowSize, const sf::Vector2f& playerVelocity) {
    m_spriteViewModel->setSprite(
        SpriteType::scoreboard,
        m_buttonColor,
        {Config::Window::RENDER_CENTER.x,
         Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50}
    );

    m_gameModel.update(playerVelocity);
    updateWater(playerVelocity);
}

void GameViewModel::updateWater(const sf::Vector2f& playerVelocity) {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= playerVelocity * Config::Game::PARALLAX_FACTOR;
    
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
