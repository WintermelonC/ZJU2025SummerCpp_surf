#include "game.h"

Game::Game()
    : m_window(sf::VideoMode(static_cast<sf::Vector2u>(Config::Window::WINDOW_SIZE)), "Surf Game"),
      m_view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(Config::Window::WINDOW_SIZE))),
      m_inputManager(m_eventBus) {
    m_view.setCenter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE / 2));  // 设置视图中心为渲染区域中心
    m_window.setView(m_view);
    m_window.setVerticalSyncEnabled(true);  // 启用垂直同步
}

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
        } else if (std::holds_alternative<WindowResizeEvent>(event)) {
            // 窗口大小调整事件
            m_view.setSize(static_cast<sf::Vector2f>(std::get<WindowResizeEvent>(event).size));
            m_window.setView(m_view);
        }
    }
}

void Game::update() {

}

void Game::render() {
    m_renderSystem.render(m_window);
}
