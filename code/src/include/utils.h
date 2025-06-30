#pragma once

#include <random>
#include <SFML/Graphics.hpp>

class Utils {
public:
    static float randomFloat(float a, float b);
    static int randomInt(int a, int b);
    static bool ifMouseOnButton(
        const sf::Vector2f& mousePos,
        const sf::Vector2f& buttonPos,
        const sf::Vector2f& buttonSize
    );

private:

};
