#pragma once

#include <SFML/Graphics.hpp>
#include "entityModel.h"

// 障碍物子类型枚举
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

enum class Ripple {
    ripple_11, ripple_21, ripple_31,
    count
};

enum class Bridge {
    bridge_1,
    count
};

enum class SpBridge {
    sp_bridge_1,
    count
};

enum class Buoy {
    buoy_1, buoy_2,
    count
};

enum class Coral {
    coral_1, coral_2, coral_3, coral_4,
    count
};

enum class LBeach {
    l_beach_1, l_beach_2, l_beach_3,
    count
};

enum class LBridge {
    l_bridge_1, l_bridge_2,
    count
};

enum class MBeach {
    m_beach_1, m_beach_2, m_beach_3, m_beach_4, m_beach_5,
    count
};

enum class MBoat {
    m_boat_1,
    count
};

enum class MBridge {
    m_bridge_1, m_bridge_2,
    count
};

enum class MMBridge {
    mm_bridge_1,
    count
};

enum class MMBeach {
    mm_beach_1, mm_beach_2, mm_beach_3,
    count
};

enum class SBuoy {
    s_buoy_1, s_buoy_2,
    count
};

// 障碍物类型枚举
enum class ObstacleType {
    none,
    wood,
    stone,
    boat,
    beach,
    seaweed,
    ripple,
    bridge,
    sp_bridge,
    buoy,
    coral,
    l_beach,
    l_bridge,
    m_beach,
    m_boat,
    m_bridge,
    mm_bridge,
    mm_beach,
    s_buoy,
    any  // 表示可以是任意类型
};

class ObstacleModel : public EntityModel {
public:
    ObstacleModel(ObstacleType obstacleType) 
        : EntityModel(EntityModelType::obstacle), m_obstacleType(obstacleType) {}

    // 静态方法 - 获取各类型障碍物数量
    static int getWoodCount() { return static_cast<int>(Wood::count); }
    static int getStoneCount() { return static_cast<int>(Stone::count); }
    static int getBoatCount() { return static_cast<int>(Boat::count); }
    static int getBeachCount() { return static_cast<int>(Beach::count); }
    static int getSeaweedCount() { return static_cast<int>(Seaweed::count); }
    static int getBridgeCount() { return static_cast<int>(Bridge::count); }
    static int getRippleCount() { return static_cast<int>(Ripple::count); }
    static int getSpBridgeCount() { return static_cast<int>(SpBridge::count); }
    static int getBuoyCount() { return static_cast<int>(Buoy::count); }
    static int getCoralCount() { return static_cast<int>(Coral::count); }
    static int getLBeachCount() { return static_cast<int>(LBeach::count); }
    static int getLBridgeCount() { return static_cast<int>(LBridge::count); }
    static int getMBeachCount() { return static_cast<int>(MBeach::count); }
    static int getMBoatCount() { return static_cast<int>(MBoat::count); }
    static int getMBridgeCount() { return static_cast<int>(MBridge::count); }
    static int getMMBridgeCount() { return static_cast<int>(MMBridge::count); }
    static int getMMBeachCount() { return static_cast<int>(MMBeach::count); }
    static int getSBuoyCount() { return static_cast<int>(SBuoy::count); }
    
    static int getObstacleCount() { 
        return getWoodCount() + getStoneCount() + getBoatCount() + 
               getBeachCount() + getSeaweedCount() + getBridgeCount() +
               getRippleCount() + getSpBridgeCount() + getBuoyCount() + 
               getCoralCount() + getLBeachCount() + getLBridgeCount() + 
               getMBeachCount() + getMBoatCount() + getMBridgeCount() +
               getMMBridgeCount() + getMMBeachCount() + getSBuoyCount();
    }

    // Getter 方法
    ObstacleType getObstacleType() const override { return m_obstacleType; }

private:
    ObstacleType m_obstacleType;  // 障碍物类型
};