#pragma once

#include "common/eventBus.h"
#include <SFML/Graphics.hpp>

class InputManager {
public:
    InputManager(EventBus& eventBus);

    void processInput(sf::RenderWindow& window);

    void reset() { m_eventBus.clear(); }

private:
    EventBus& m_eventBus;
};
