#include "game.h"

Game::Game()
    : m_window(sf::VideoMode(static_cast<sf::Vector2u>(Config::Window::WINDOW_SIZE)), "Surf Game"),
      m_view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(Config::Window::WINDOW_SIZE))),
      m_inputManager(m_eventBus) {}

void Game::run() {
    while (m_window.isOpen()) {
        // 接收输入
        m_inputManager.processInput(m_window);

        // 处理游戏事件
        processGameEvents();

        // 渲染
        render();
    }
}

void Game::processGameEvents() {
    Event event;
    while (m_eventBus.poll(event)) {
        if (std::holds_alternative<WindowCloseEvent>(event)) {
            // 处理窗口关闭事件
            m_window.close();
        }
    }
}

void Game::update() {

}

void Game::render() {
    m_renderSystem.render(m_window);
}
