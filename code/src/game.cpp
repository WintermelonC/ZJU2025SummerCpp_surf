#include "include/game.h"

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Surf Game"),
      m_view(sf::FloatRect({RENDER_CENTER_X, RENDER_CENTER_Y}, {WINDOW_WIDTH, WINDOW_HEIGHT})),
      m_bgTexture(m_bgPath),
      m_bgSprite(m_bgTexture),
      m_player({RENDER_CENTER_X, RENDER_CENTER_Y}) {
    // 初始化视图
    updateView();
    // 激活垂直同步
    m_window.setVerticalSyncEnabled(true);

    // 纹理重复平铺
    m_bgTexture.setRepeated(true);
    // 设置精灵纹理矩形
    m_bgSprite.setTextureRect(sf::IntRect({RENDER_CENTER_X, RENDER_CENTER_Y}, {RENDER_WIDTH, RENDER_HEIGHT}));
}

void Game::run() {
    while (m_window.isOpen()) {
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
            m_view.setSize(sf::Vector2f(resized -> size));
        }
    }
}

void Game::update() {
    updateView();
}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口
    m_window.draw(m_bgSprite);  // 绘制背景精灵
    m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    m_window.display();  // 显示渲染结果
}

void Game::updateView() {
    // 更新视图中心为玩家位置
    m_view.setCenter(m_player.getPosition());
    m_window.setView(m_view);
}
