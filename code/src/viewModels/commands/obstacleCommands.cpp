#include "../obstacleItemViewModel.h"

std::function<void(const float&, const sf::Sprite&)> ObstacleItemViewModel::getUpdateCommand() {
    return [this](const float& dt, const sf::Sprite& playerSprite) {
        update(dt);
        checkCollisionWithPlayer(playerSprite);
    };
}