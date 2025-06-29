#include "inputManager.h"

InputManager::InputManager(EventBus& eventBus)
    : m_eventBus(eventBus) {}

void InputManager::processInput(sf::RenderWindow& window) {
    while(const std::optional event = window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            // 处理窗口关闭事件
            m_eventBus.publish(WindowCloseEvent{});
        } else if (const auto* resized = event -> getIf<sf::Event::Resized>()) {
            // 窗口大小调整事件
            m_eventBus.publish(WindowResizeEvent{resized -> size});
        } else if (event -> is<sf::Event::FocusLost>()) {
            // 窗口失去焦点事件
            m_eventBus.publish(WindowFocusLostEvent{});
        } else if (event -> is<sf::Event::FocusGained>()) {
            // 窗口获得焦点事件
            m_eventBus.publish(WindowFocusGainedEvent{});
        } else if (const auto* mouseButton = event -> getIf<sf::Event::MouseButtonPressed>()) {
            // 鼠标按钮按下事件
            if (mouseButton -> button == sf::Mouse::Button::Left) {
                // 左键点击事件
                const auto& mousePos = sf::Mouse::getPosition(window);
                const auto worldPos = window.mapPixelToCoords(mousePos);
                m_eventBus.publish(MouseLeftClickEvent{
                    mousePos,
                    worldPos
                });
            } else if (mouseButton -> button == sf::Mouse::Button::Right) {
                // 右键点击事件
                m_eventBus.publish(MouseRightClickEvent{});
            }
        } else if (const auto* keyPressed = event -> getIf<sf::Event::KeyPressed>()){
            // 键盘按下
            if (keyPressed -> code == sf::Keyboard::Key::Space) {
                // 空格键按下事件
                m_eventBus.publish(SpacePressedEvent{});
            }
        }
    }
}
