#include "../obstacleItemViewModel.h"

Config::ObstacleItemUpdateCallback ObstacleItemViewModel::getUpdateCommand() {
    return [this](const float& dt) {
        update(dt);
    };
}