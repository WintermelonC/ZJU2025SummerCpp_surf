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

enum class Seaweed {
    seaweed_11, seaweed_21, seaweed_31,
    count
};

enum class Bridge {
    bridge_1,
    count
};

// 添加障碍物类型枚举
enum class ObstacleType {
    none,
    wood,
    stone,
    boat,
    beach,
    seaweed,
    bridge,
    any  // 表示可以是任意类型
};

class ObstacleModel : public EntityModel {
public:
    ObstacleModel(ObstacleType obstacleType) : EntityModel(EntityModelType::obstacle), m_obstacleType(obstacleType) {}

    static int getWoodCount() { return static_cast<int>(Wood::count); }
    static int getStoneCount() { return static_cast<int>(Stone::count); }
    static int getBoatCount() { return static_cast<int>(Boat::count); }
    static int getBeachCount() { return static_cast<int>(Beach::count); }
    static int getSeaweedCount() { return static_cast<int>(Seaweed::count); }
    static int getBridgeCount() { return static_cast<int>(Bridge::count); }
    static int getObstacleCount() { return getWoodCount() + getStoneCount() + getBoatCount() + getBeachCount() + getSeaweedCount() + getBridgeCount(); }

    ObstacleType getObstacleType() const override { return m_obstacleType; }

private:
    ObstacleType m_obstacleType;  // 障碍物类型

};