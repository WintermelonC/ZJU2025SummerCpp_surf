#pragma once

#include <random>
#include <SFML/Graphics.hpp>

class Utils {
public:
    static bool ifMouseOnButton(
        const sf::Vector2f& mousePos,
        const sf::Vector2f& buttonPos,
        const sf::Vector2f& buttonSize
    ) {
        return (
        mousePos.x >= buttonPos.x - buttonSize.x / 2 && 
        mousePos.x <= buttonPos.x + buttonSize.x / 2 &&
        mousePos.y >= buttonPos.y - buttonSize.y / 2 && 
        mousePos.y <= buttonPos.y + buttonSize.y / 2
    );
    }

private:

};
