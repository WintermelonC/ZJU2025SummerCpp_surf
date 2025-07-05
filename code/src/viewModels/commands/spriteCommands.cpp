#include "../spriteViewModel.h"

std::function<void(const sf::Vector2u&)> SpriteViewModel::getUpdateCommand() {
    return [this](const sf::Vector2u& windowSize) {
        update(windowSize);
    };
}