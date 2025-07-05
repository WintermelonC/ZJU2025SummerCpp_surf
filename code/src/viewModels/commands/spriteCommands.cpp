#include "../spriteViewModel.h"

Config::SpriteUpdateCallback SpriteViewModel::getUpdateCommand() {
    return [this](const sf::Vector2u& windowSize) {
        update(windowSize);
    };
}