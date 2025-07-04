#include "gameViewModel.h"

GameViewModel::GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM)
    : m_spriteViewModel(spriteVM),
      m_ObstacleItemViewModel(spriteVM),
      m_playerViewModel(spriteVM) {
    m_spriteViewModel->setSprite(
        SpriteType::water,
        sf::Color::White,
        {0, 0},
        {1, 1},
        false
    );
}

void GameViewModel::update(const sf::Vector2f& mousePos, const sf::Vector2u& windowSize) {
    float deltaTime = m_clock.restart().asSeconds();
    m_spriteViewModel->setSprite(
        SpriteType::scoreboard,
        m_buttonColor,
        {Config::Window::RENDER_CENTER.x,
         Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50}
    );

    m_gameModel.update(m_playerViewModel.getPlayerVelocity());
    m_playerViewModel.update(deltaTime, mousePos);
    m_ObstacleItemViewModel.update(deltaTime, m_playerViewModel.getPlayerVelocity(), !m_playerViewModel.isPlayerStop());
    updateWater();
}

void GameViewModel::handleMouseEvents(const sf::Event::MouseButtonPressed& mouseButton) {
    if (mouseButton.button == sf::Mouse::Button::Right) {
        m_playerViewModel.usePower();
    }
}

void GameViewModel::updateWater() {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= m_playerViewModel.getPlayerVelocity() * Config::Game::PARALLAX_FACTOR;
    
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
