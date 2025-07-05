#include "../obstacleItemViewModel.h"

std::function<void(const float&)> ObstacleItemViewModel::getUpdateCommand() {
    return [this](const float& dt) {
        update(dt);
    };
}