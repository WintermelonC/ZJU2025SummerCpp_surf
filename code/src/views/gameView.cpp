#include "views/gameView.h"
#include "views/entityManager.h"
#include "views/renderSystem.h"
#include "infrastructure/config.h"

SFMLGameView::SFMLGameView()
    : m_window(sf::VideoMode(static_cast<sf::Vector2u>(Config::Window::WINDOW_SIZE)), "Surf Game"),
      m_view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(Config::Window::WINDOW_SIZE))),
      m_waterOffset({0.0f, 0.0f}) {
    
    m_view.setCenter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE / 2));
    m_window.setView(m_view);
    m_window.setVerticalSyncEnabled(true);
}

void SFMLGameView::renderStartMenu() {
    clear();
    renderBackground();
    // 渲染开始菜单UI
    RenderSystem renderSystem;
    renderSystem.renderStartMenu(m_window);
}

void SFMLGameView::renderGameplay() {
    clear();
    renderBackground();
    renderWater();
    renderPlayer();
    renderUI();
    
    // 渲染水波和拖尾效果
    RenderSystem renderSystem;
    renderSystem.renderRipple(m_window);
    renderSystem.renderTail(m_window);
    
#ifdef DEBUG
    renderSystem.renderVelocity(m_window, m_playerData.getVelocity());
#endif
}

void SFMLGameView::renderPauseMenu() {
    // 先渲染游戏画面作为背景
    renderGameplay();
    
    // 然后渲染暂停菜单
    RenderSystem renderSystem;
    renderSystem.renderPauseMenu(m_window);
}

void SFMLGameView::renderGameOver() {
    clear();
    renderBackground();
    // TODO: 实现游戏结束界面
}

void SFMLGameView::display() {
    m_window.display();
}

void SFMLGameView::clear() {
    m_window.clear(sf::Color(0, 192, 222));
}

bool SFMLGameView::isOpen() const {
    return m_window.isOpen();
}

sf::Vector2i SFMLGameView::getMousePosition() const {
    return sf::Mouse::getPosition(m_window);
}

sf::Vector2f SFMLGameView::mapPixelToCoords(const sf::Vector2i& pixel) const {
    return m_window.mapPixelToCoords(pixel);
}

void SFMLGameView::close() {
    m_window.close();
}

void SFMLGameView::updatePlayerData(const PlayerModel& player) {
    m_playerData = player;
}

void SFMLGameView::updateGameData(const GameModel& game) {
    m_gameData = game;
}

void SFMLGameView::updateWaterOffset(const sf::Vector2f& offset) {
    m_waterOffset = offset;
}

void SFMLGameView::renderBackground() {
    // 渲染背景色已在clear()中完成
}

void SFMLGameView::renderPlayer() {
    RenderSystem renderSystem;
    renderSystem.renderPlayer(m_window, m_playerData.getHP(), m_playerData.getPower(), m_gameData.getScore());
}

void SFMLGameView::renderUI() {
    // UI渲染已在renderPlayer中包含
}

void SFMLGameView::renderWater() {
    // 更新水面精灵位置
    EntityManager::setSprite(
        EntityType::water,
        m_waterOffset,
        {1.0f, 1.0f},
        false,
        {Config::Texture::WATER_SIZE.x, 0.f}
    );
    
    sf::Sprite& water = EntityManager::getSprite(EntityType::water);
    m_window.draw(water);
}
