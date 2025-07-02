#pragma once

#include <functional>
#include <memory>
#include "common/config.h"

enum class GameState {
    StartMenu,
    Playing,
    Paused,
    GameOver
};

// 游戏数据模型
class GameModel {
public:
    GameModel();
    
    // 获取器
    GameState getGameState() const { return m_gameState; }
    float getScore() const { return m_score; }
    const sf::Vector2f& getWaterOffset() const { return m_waterOffset; }
    
    // 设置器
    void setGameState(GameState state);
    void updateScore(float deltaScore);
    void updateWaterOffset(const sf::Vector2f& velocityDelta);
    void resetGame();
    
    // 事件回调
    void setOnStateChanged(std::function<void(GameState)> callback) { 
        m_onStateChanged = callback; 
    }
    void setOnScoreChanged(std::function<void(float)> callback) { 
        m_onScoreChanged = callback; 
    }

private:
    GameState m_gameState;
    float m_score;
    sf::Vector2f m_waterOffset;
    
    // 事件回调
    std::function<void(GameState)> m_onStateChanged;
    std::function<void(float)> m_onScoreChanged;
};
