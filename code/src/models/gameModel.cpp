#include "models/gameModel.h"

GameModel::GameModel() 
    : m_gameState(GameState::StartMenu),
      m_score(0.0f),
      m_waterOffset({0.0f, 0.0f}) {
}

void GameModel::setGameState(GameState state) {
    if (m_gameState != state) {
        m_gameState = state;
        if (m_onStateChanged) {
            m_onStateChanged(state);
        }
    }
}

void GameModel::updateScore(float deltaScore) {
    m_score += deltaScore;
    if (m_onScoreChanged) {
        m_onScoreChanged(m_score);
    }
}

void GameModel::updateWaterOffset(const sf::Vector2f& velocityDelta) {
    // 根据玩家移动方向反向移动水面
    m_waterOffset -= velocityDelta * Config::Game::PARALLAX_FACTOR;
    
    if (m_waterOffset.x <= 0) {
        m_waterOffset.x += Config::Texture::WATER_SIZE.x;
    } else if (m_waterOffset.x >= Config::Texture::WATER_SIZE.x) {
        m_waterOffset.x -= Config::Texture::WATER_SIZE.x;
    }
    
    if (m_waterOffset.y <= -Config::Texture::WATER_SIZE.y) {
        m_waterOffset.y += Config::Texture::WATER_SIZE.y;
    }
}

void GameModel::resetGame() {
    m_score = 0.0f;
    m_waterOffset = {0.0f, 0.0f};
    setGameState(GameState::StartMenu);
}
