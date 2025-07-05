#include "../playerViewModel.h"

std::function<void()> PlayerViewModel::getMouseRightClickCommand() {
    return [this]() {
        usePower();
    };
}

std::function<void(const float&, const sf::Vector2f&)> PlayerViewModel::getUpdateCommand() {
    return [this](const float& deltaTime, const sf::Vector2f& mousePos) {
        update(deltaTime, mousePos);
    };
}