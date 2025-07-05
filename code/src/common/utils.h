#pragma once

#include <random>
#include <SFML/Graphics.hpp>

class Utils {
public:
    static float randomFloat(float a, float b) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(a, b);
        return dist(rng);
    }
    static int randomInt(int a, int b) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(a, b);
        return dist(rng);
    }
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