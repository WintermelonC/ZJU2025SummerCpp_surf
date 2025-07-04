#include "../gameViewModel.h"

std::function<void()> GameViewModel::getFocusLostCommand() {
    return [this]() {
        if( m_gameModel.getGameState() == GameState::playing) {
            m_gameModel.setGameState(GameState::paused);
        }
    };
}

std::function<void()> GameViewModel::getFocusGainedCommand() {
    return [this]() {
        if( m_gameModel.getGameState() == GameState::paused) {
            m_gameModel.setGameState(GameState::playing);
        }
    };
}

std::function<void(const bool&, const bool&,
                   const bool&)> GameViewModel::getMouseLeftClickCommand() {
    return [this](const bool& StartButtonPressed, 
                  const bool& ContinueButtonPressed,
                  const bool& ReturnButtonPressed) {
        GameState currentState = m_gameModel.getGameState();
        if (currentState == GameState::startMenu) {
            if (StartButtonPressed) {
                m_gameModel.setGameState(GameState::playing);
            }
        } else if (currentState == GameState::paused) {
            if (ContinueButtonPressed) {
                m_gameModel.setGameState(GameState::playing);
            } else if (ReturnButtonPressed) {
                // reset有待实现
            }
        }
    };
}

std::function<void(const sf::Event::KeyPressed&)> GameViewModel::getKeyPressCommand() {
    return [this](const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.code == sf::Keyboard::Key::Space) {
            GameState currentState = m_gameModel.getGameState();
            if (currentState == GameState::playing) {
                m_gameModel.setGameState(GameState::paused); 
            } else if (currentState == GameState::paused) {
                m_gameModel.setGameState(GameState::playing);
            }
        }
    };
}