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
        }
    }
}
