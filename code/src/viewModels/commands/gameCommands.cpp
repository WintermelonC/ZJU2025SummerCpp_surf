#include "../gameViewModel.h"

std::function<void()> GameViewModel::getFocusLostCommand() {
    return [this]() {
        if( m_gameModel.getGameState() == Config::GameState::playing) {
            m_gameModel.setGameState(Config::GameState::paused);
        }
    };
}

std::function<void()> GameViewModel::getFocusGainedCommand() {
    return [this]() {
        if( m_gameModel.getGameState() == Config::GameState::paused) {
            m_gameModel.setGameState(Config::GameState::playing);
        }
    };
}

std::function<void(const bool&, const bool&,
                   const bool&)> GameViewModel::getMouseLeftClickCommand() {
    return [this](const bool& StartButtonPressed, 
                  const bool& ContinueButtonPressed,
                  const bool& ReturnButtonPressed) {
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
        }
    };
}

std::function<void(const sf::Event::KeyPressed&)> GameViewModel::getKeyPressCommand() {
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

std::function<void(const sf::Vector2u&)> GameViewModel::getUpdateCommand() {
    return [this](const sf::Vector2u& windowSize) {
        update(windowSize);
    };
}