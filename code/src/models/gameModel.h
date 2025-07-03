#pragma once

enum class GameState {
    startMenu,
    playing,
    paused,
    gameOver
};

class GameModel {
public:
    void reset() {
        m_gameState = GameState::startMenu;
        m_score = 0;
    }

    const GameState& getGameState() const { return m_gameState; }
    const float& getScore() const { return m_score; }

    void setGameState(GameState state) { m_gameState = state; }
    void setScore(float score) { m_score = score; }

private:
    GameState m_gameState = GameState::startMenu;
    float m_score = 0;
};