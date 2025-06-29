#include "game.h"

Game::Game()
    : m_window(sf::VideoMode(static_cast<sf::Vector2u>(Config::Window::WINDOW_SIZE)), "Surf Game"),
      m_view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(Config::Window::WINDOW_SIZE))),
      m_inputManager(m_eventBus),
      m_offset({Config::Texture::WATER_SIZE.x, 0.0f}) {
    m_view.setCenter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE / 2));  // 设置视图中心为渲染区域中心
    m_window.setView(m_view);
    m_window.setVerticalSyncEnabled(true);  // 启用垂直同步
}

void Game::run() {
    while (m_window.isOpen()) {
        m_inputManager.processInput(m_window);  // 接收输入
        processGameEvents();  // 处理游戏事件
        update();  // 更新游戏状态
        render();  // 渲染
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
        } else if (std::holds_alternative<MouseRightClickEvent>(event)) {
            // 鼠标右键点击事件
            m_player.usePower();  // 玩家使用能量
        }
    }
}

void Game::update() {
    const float dt = m_clock.restart().asSeconds();  // 获取帧时间间隔
    const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));  // 获取鼠标位置
    const bool ifSpawnRipple = m_player.isTrun() && m_player.getVelocity().y > SPEED_THRESHOLD_1;  // 判断是否生成水波
    const bool ifSpawnTail = m_player.isPower() && m_player.getVelocity().y > SPEED_THRESHOLD_2;  // 判断是否生成拖尾
    
    updateWater();  // 更新水面状态
    m_renderSystem.updateRipple(dt, m_player.getVelocity(), ifSpawnRipple);  // 更新水波状态
    m_renderSystem.updateTail(dt, m_player.getVelocity(), m_player.getAngle(), ifSpawnTail);  // 更新拖尾状态
    m_player.update(dt, mousePos);  // 更新玩家状态
    updateScore();  // 更新分数
}

void Game::render() {
    m_renderSystem.render(m_window);
    m_renderSystem.renderPlayerState(m_window, m_player.getHP(), m_player.getPower(), m_score);  // 渲染玩家状态
#ifdef DEBUG
    m_renderSystem.renderVelocity(m_window, m_player.getVelocity());  // 渲染玩家速度
#endif  // DEBUG
    m_window.display();  // 显示渲染结果
}

void Game::updateWater() {
    // 根据玩家移动方向反向移动水面
    m_offset -= m_player.getVelocity() * Config::Game::PARALLAX_FACTOR;
    if (m_offset.x <= 0) {
        m_offset.x += Config::Texture::WATER_SIZE.x;
    } else if (m_offset.x >= Config::Texture::WATER_SIZE.x) {
        m_offset.x -= Config::Texture::WATER_SIZE.x;
    }
    if (m_offset.y <= -Config::Texture::WATER_SIZE.y) {
        m_offset.y += Config::Texture::WATER_SIZE.y;
    }
    EntityManager::setSprite(
        EntityType::water, 
        m_offset, 
        {1.0f, 1.0f}, 
        false, 
        {Config::Texture::WATER_SIZE.x, 0.f}
    );
}

void Game::updateScore() {
    m_score += m_player.getVelocity().y * 0.001f;
}
