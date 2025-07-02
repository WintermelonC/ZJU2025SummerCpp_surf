#include "views/gameView.h"
#include "views/spriteManager.h"
#include "views/renderSystem.h"
#include "common/config.h"
#include "common/eventBus.h"

SFMLGameView::SFMLGameView()
    : m_window(sf::VideoMode(static_cast<sf::Vector2u>(Config::Window::WINDOW_SIZE)), "Surf Game"),
      m_view(sf::FloatRect({0, 0}, static_cast<sf::Vector2f>(Config::Window::WINDOW_SIZE)))
       {
    
    m_view.setCenter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE / 2));
    m_window.setView(m_view);
    m_window.setVerticalSyncEnabled(true);
}

void SFMLGameView::renderStartMenu(const sf::Vector2f& waterOffset) {
    clear();
    renderWater(waterOffset);
    // 渲染开始菜单UI
    m_renderSystem.renderStartMenu(m_window);
}

void SFMLGameView::renderGameplay(
    float deltaTime,
    const sf::Vector2f& waterOffset, 
    const int & hp, 
    const int & power, 
    const float & score
) {
    clear();
    renderWater(waterOffset);
    // 渲染水波和拖尾效果
    m_renderSystem.renderRipple(m_window);
    m_renderSystem.renderTail(m_window);
    
    // 渲染玩家
    renderPlayer(hp, power, score);
}

void SFMLGameView::renderPauseMenu(
    float deltaTime,
    const sf::Vector2f& waterOffset,
    const int & hp,
    const int & power,
    const float & score
) {
    // 先渲染游戏画面作为背景
    renderGameplay(
        deltaTime, 
        waterOffset, 
        hp, 
        power, 
        score
    );
    
    // 然后渲染暂停菜单
    m_renderSystem.renderPauseMenu(m_window);
}

void SFMLGameView::renderGameOver() {
    clear();
    // TODO: 实现游戏结束界面
}

void SFMLGameView::display() {
    m_window.display();
}

void SFMLGameView::clear() {
    m_window.clear(sf::Color(0, 192, 222));
}

void SFMLGameView::reset() {
    m_window.setView(m_view);
    
    // 重置渲染系统状态
    m_renderSystem.reset();
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

void SFMLGameView::updateEffects(float deltaTime, const sf::Vector2f& velocity, const sf::Angle& angle, bool shouldSpawnRipple, bool shouldSpawnTail) {
    // 更新水波特效
    m_renderSystem.updateRipple(deltaTime, velocity, angle, shouldSpawnRipple);
    
    // 更新拖尾特效
    m_renderSystem.updateTail(deltaTime, velocity, angle, shouldSpawnTail);
}

void SFMLGameView::renderPlayer(const int & hp, const int & power, const float & score) {
    m_renderSystem.renderPlayer(m_window, hp, power, score);
}

void SFMLGameView::renderWater(const sf::Vector2f& waterOffset) {
    // 更新水面精灵位置
    SpriteManager::setSprite(
        SpriteType::water,
        waterOffset,
        {1.0f, 1.0f},
        false,
        {Config::Texture::WATER_SIZE.x, 0.f}
    );
    
    sf::Sprite& water = SpriteManager::getSprite(SpriteType::water);
    m_window.draw(water);
}

void SFMLGameView::processInput(EventBus& eventBus) {
    while(const std::optional event = m_window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            // 处理窗口关闭事件
            eventBus.publish(WindowCloseEvent{});
        } else if (const auto* resized = event -> getIf<sf::Event::Resized>()) {
            // 窗口大小调整事件
            eventBus.publish(WindowResizeEvent{resized -> size});
        } else if (event -> is<sf::Event::FocusLost>()) {
            // 窗口失去焦点事件
            eventBus.publish(WindowFocusLostEvent{});
        } else if (event -> is<sf::Event::FocusGained>()) {
            // 窗口获得焦点事件
            eventBus.publish(WindowFocusGainedEvent{});
        } else if (const auto* mouseButton = event -> getIf<sf::Event::MouseButtonPressed>()) {
            // 鼠标按钮按下事件
            if (mouseButton -> button == sf::Mouse::Button::Left) {
                // 左键点击事件
                const auto& mousePos = sf::Mouse::getPosition(m_window);
                const auto worldPos = m_window.mapPixelToCoords(mousePos);
                eventBus.publish(MouseLeftClickEvent{
                    mousePos,
                    worldPos
                });
            } else if (mouseButton -> button == sf::Mouse::Button::Right) {
                // 右键点击事件
                eventBus.publish(MouseRightClickEvent{});
            }
        } else if (const auto* keyPressed = event -> getIf<sf::Event::KeyPressed>()){
            // 键盘按下
            if (keyPressed -> code == sf::Keyboard::Key::Space) {
                // 空格键按下事件
                eventBus.publish(SpacePressedEvent{});
            }
        }
    }
}

void SFMLGameView::setViewSize(const sf::Vector2f& size) {
    m_view.setSize(size);
    m_window.setView(m_view);
}
