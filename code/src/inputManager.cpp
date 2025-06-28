#include "inputManager.h"

InputManager::InputManager(EventBus& eventBus)
    : m_eventBus(eventBus) {}

void InputManager::processInput(sf::RenderWindow& window) {
    while(const std::optional event = window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            m_eventBus.publish(WindowCloseEvent{});
        }
    }
}
