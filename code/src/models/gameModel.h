#pragma once

#include "../common/config.h"

class GameModel {
public:
    // 重置游戏状态
    void reset() {
        m_gameState = Config::GameState::startMenu;
        m_score = 0;
    }

    // 更新游戏状态
    void update(const sf::Vector2f& playerVelocity) { 
        m_score += playerVelocity.y * 0.001f; 
    }

    // Getter 方法
    const Config::GameState& getGameState() const { return m_gameState; }
    const float& getScore() const { return m_score; }

    // Setter 方法
    void setGameState(Config::GameState state) { m_gameState = state; }
    void setScore(float score) { m_score = score; }

private:
    Config::GameState m_gameState = Config::GameState::startMenu;
    float m_score = 0;
};