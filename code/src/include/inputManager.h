#pragma once

#include "eventBus.h"
#include <SFML/Graphics.hpp>

class InputManager {
public:
    InputManager(EventBus& eventBus);

    void processInput(sf::RenderWindow& window);

private:
    EventBus& m_eventBus;
};
