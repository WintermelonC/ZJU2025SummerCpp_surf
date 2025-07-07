#pragma once

#include "../common/config.h"
#include "../common/scoreManager.h"

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
    
    // 游戏结束时调用，检查并更新最高分
    bool gameOver() {
        bool isNewHighScore = ScoreManager::getInstance().updateHighScoreIfNeeded(m_score);
        return isNewHighScore;
    }

    // Getter 方法
    const Config::GameState& getGameState() const { return m_gameState; }
    const float& getScore() const { return m_score; }
    float getHighScore() const { return ScoreManager::getInstance().getHighScore(); }

    // Setter 方法
    void setGameState(Config::GameState state) { m_gameState = state; }
    void setScore(float score) { m_score = score; }

private:
    Config::GameState m_gameState = Config::GameState::startMenu;
    float m_score = 0;
};