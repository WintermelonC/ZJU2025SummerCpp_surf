#include "../playerViewModel.h"

Config::MouseRightClickCallback PlayerViewModel::getMouseRightClickCommand() {
    return [this]() {
        usePower();
    };
}

Config::PlayerUpdateCallback PlayerViewModel::getUpdateCommand() {
    return [this](const float& deltaTime, const sf::Vector2f& mousePos, const sf::Vector2u& windowSize) {
        update(deltaTime, mousePos, windowSize);
    };
}

Config::MouseLeftClickPlayerWaitingCallback PlayerViewModel::getMouseLeftClickPlayerWaitingCommand() {
    return [this]() {
        updateWaiting();
    };
}