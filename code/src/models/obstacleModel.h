#pragma once

#include <SFML/Graphics.hpp>
#include "entityModel.h"

enum class Wood {
    wood_1, wood_2, wood_3, wood_4, wood_5, wood_6,
    count
};

enum class Stone {
    stone_1, stone_2, stone_3, stone_4, stone_5,
    count
};

enum class Boat {
    boat_1, boat_2,
    count
};

enum class Beach {
    beach_1, beach_2, beach_3,
    count
};

// 添加障碍物类型枚举
enum class ObstacleType {
    wood,
    stone,
    boat,
    beach,
    any  // 表示可以是任意类型
};

class ObstacleModel : public EntityModel {
public:
    ObstacleModel() : EntityModel(EntityModelType::obstacle) {}

    int getWoodCount() const { return static_cast<int>(Wood::count); }
    int getStoneCount() const { return static_cast<int>(Stone::count); }
    int getBoatCount() const { return static_cast<int>(Boat::count); }
    int getBeachCount() const { return static_cast<int>(Beach::count); }
    int getObstacleCount() const { return getWoodCount() + getStoneCount() + getBoatCount() + getBeachCount(); }

private:
};