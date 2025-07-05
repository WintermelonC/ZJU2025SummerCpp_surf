#include "../playerViewModel.h"

Config::MouseRightClickCallback PlayerViewModel::getMouseRightClickCommand() {
    return [this]() {
        usePower();
    };
}

Config::PlayerUpdateCallback PlayerViewModel::getUpdateCommand() {
    return [this](const float& deltaTime, const sf::Vector2f& mousePos) {
        update(deltaTime, mousePos);
    };
}