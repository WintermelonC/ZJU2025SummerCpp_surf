#include "include/game.h"

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Surf Game"),
      m_view(sf::FloatRect({0.f, 0.f}, {WINDOW_WIDTH, WINDOW_HEIGHT})),
      m_isRunning(true),
      m_backgroundTexture(m_backgroundPath),
      m_background(m_backgroundTexture) {
    
    m_window.setView(m_view);
    // 激活垂直同步
    m_window.setVerticalSyncEnabled(true);

    // 纹理重复平铺
    m_backgroundTexture.setRepeated(true);
    // 设置精灵纹理矩形
    m_background.setTextureRect(sf::IntRect({0, 0}, {RENDER_WIDTH, RENDER_HEIGHT}));
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
            // 关闭窗口事件
            m_window.close();
        } else if (const auto* resized = event -> getIf<sf::Event::Resized>()) {
            // 窗口大小调整事件
            sf::FloatRect visibleArea({0.f, 0.f}, sf::Vector2f(resized -> size));
            m_window.setView(sf::View(visibleArea));
        }
    }
}

void Game::update() {

}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口
    m_window.draw(m_background);  // 绘制背景精灵
    m_window.display();  // 显示渲染结果
}
