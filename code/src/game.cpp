#include "include/game.h"

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Surf Game"),
      m_view(sf::FloatRect({0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT})),
      m_isRunning(true) {
    
    m_window.setView(m_view);
}

void Game::run() {
    while (m_isRunning && m_window.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

void Game::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            m_window.close();
        }
    }
}

void Game::update() {

}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));
    m_window.display();
}
