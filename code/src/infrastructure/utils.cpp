#include "infrastructure/utils.h"

float Utils::randomFloat(float a, float b) {
    static std::mt19937 rng(std::random_device{}());
    std::uniform_real_distribution<float> dist(a, b);
    return dist(rng);
}

bool Utils::ifMouseOnButton(
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
