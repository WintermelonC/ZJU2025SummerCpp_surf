#include "inputManager.h"

InputManager::InputManager(EventBus& eventBus)
    : m_eventBus(eventBus) {}

void InputManager::processInput(sf::RenderWindow& window) {
    while(const std::optional event = window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            // 发布窗口关闭事件
            m_eventBus.publish(WindowCloseEvent{});
        } else if (const auto* resized = event -> getIf<sf::Event::Resized>()) {
            // 发布窗口大小调整事件
            m_eventBus.publish(WindowResizeEvent{resized -> size});
        } else if (const auto* mouseButton = event -> getIf<sf::Event::MouseButtonPressed>()) {
            // 检测鼠标点击事件
            if (mouseButton -> button == sf::Mouse::Button::Right) {
                // 发布鼠标右键点击事件
                m_eventBus.publish(MouseRightClickEvent{});
            }
        }
    }
}
