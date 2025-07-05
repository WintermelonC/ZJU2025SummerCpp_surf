#pragma once

#include "../common/config.h"

class GameModel {
public:
    void reset() {
        m_gameState = Config::GameState::startMenu;
        m_score = 0;
    }
    void update(const sf::Vector2f& playerVelocity) { m_score += playerVelocity.y * 0.001f; }

    const Config::GameState& getGameState() const { return m_gameState; }
    const float& getScore() const { return m_score; }

    void setGameState(Config::GameState state) { m_gameState = state; }
    void setScore(float score) { m_score = score; }

private:
    Config::GameState m_gameState = Config::GameState::startMenu;
    float m_score = 0;
};