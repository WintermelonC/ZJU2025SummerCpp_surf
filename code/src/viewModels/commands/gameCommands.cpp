#include "../gameViewModel.h"

Config::FocusLostCallback GameViewModel::getFocusLostCommand() {
    return [this]() {
        if( m_gameModel.getGameState() == Config::GameState::playing) {
            m_gameModel.setGameState(Config::GameState::paused);
        }
    };
}

Config::FocusGainedCallback GameViewModel::getFocusGainedCommand() {
    return [this]() {
        // nothing to do
    };
}

Config::MouseLeftClickCallback GameViewModel::getMouseLeftClickCommand() {
    return [this](const bool& StartButtonPressed, 
                  const bool& ContinueButtonPressed,
                  const bool& ReturnButtonPressed,
                  const bool& NewGameButtonPressed) {
        Config::GameState currentState = m_gameModel.getGameState();
        if (currentState == Config::GameState::startMenu) {
            if (StartButtonPressed) {
                m_gameModel.setGameState(Config::GameState::playing);
            }
        } else if (currentState == Config::GameState::paused) {
            if (ContinueButtonPressed) {
                m_gameModel.setGameState(Config::GameState::playing);
            } else if (ReturnButtonPressed) {
                resetGame();
                m_gameModel.setGameState(Config::GameState::startMenu);
            }
        } else if (currentState == Config::GameState::gameOver) {
            if (NewGameButtonPressed) {
                resetGame();
                m_gameModel.setGameState(Config::GameState::playing);
            } else if (ReturnButtonPressed) {
                resetGame();
                m_gameModel.setGameState(Config::GameState::startMenu);
            }
        } else if (currentState == Config::GameState::playing && *m_isPlayerWaiting) {
            if (m_mouseLeftClickPlayerWaitingCallback) {
                m_mouseLeftClickPlayerWaitingCallback();
            }
        }
    };
}

Config::KeyPressCallback GameViewModel::getKeyPressCommand() {
    return [this](const sf::Event::KeyPressed& keyPressed) {
        if (keyPressed.code == sf::Keyboard::Key::Space) {
            Config::GameState currentState = m_gameModel.getGameState();
            if (currentState == Config::GameState::playing) {
                m_gameModel.setGameState(Config::GameState::paused); 
            } else if (currentState == Config::GameState::paused) {
                m_gameModel.setGameState(Config::GameState::playing);
            }
        }
    };
}

Config::GameUpdateCallback GameViewModel::getUpdateCommand() {
    return [this](const sf::Vector2u& windowSize) {
        update(windowSize);
    };
}