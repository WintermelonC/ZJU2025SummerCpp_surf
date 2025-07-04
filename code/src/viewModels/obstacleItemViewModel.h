#pragma once

#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../models/itemModel.h"
#include "../models/obstacleModel.h"
#include "textureViewModel.h"
#include "spriteViewModel.h"
#include "../common/config.h"
#include "../common/notificationCenter.h"

// 生成项结构，统一管理障碍物和道具
struct SpawnItem {
    enum class Type { Obstacle, Item };
    Type type;
    
    // 联合体，节省内存
    union {
        ObstacleType obstacleType;  // 当 type 为 Obstacle 时使用
        ItemType itemType;          // 当 type 为 Item 时使用
    };
    
    // 构造函数
    SpawnItem(ObstacleType obsType) 
        : type(Type::Obstacle), obstacleType(obsType) {}
    
    SpawnItem(ItemType itType) 
        : type(Type::Item), itemType(itType) {}
};

class ObstacleItemViewModel : public INotificationObserver {
public:
    ObstacleItemViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const float& dt, const sf::Vector2f& playerVelocity, const bool isSpawn = true);
    std::vector<sf::Sprite>& getSprites() { return m_sprites; }
    
    // 🔔 实现观察者接口
    void onNotification(const NotificationData& data) override;

private:
    void initialize();
    void updatePosition(const sf::Vector2f& playerVelocity);
    void resetObstacles(); // 🔔 内部重置方法

    void spawnSingle();
    void spawnGroup();

    void createClusterPattern();     // 聚集模式

    // 随机选择障碍物纹理
    TextureType getRandomObstacleTexture(ObstacleType type);

private:
    const float m_spawnInterval = 0.5f;  // 生成间隔时间

    // 障碍物和道具组合模式结构
    struct Pattern {
        std::vector<SpawnItem> items;  // 包含障碍物和道具的生成项
        std::vector<sf::Vector2f> positions;  // 相对位置
        float width;  // 模式宽度
        float height;  // 模式高度
    };
    
    std::vector<Pattern> m_patterns;  // 预定义的障碍物和道具组合模式
    sf::Clock m_spawnClock;  // 生成时钟
    
    // 随机数生成器
    std::random_device m_rd;
    std::mt19937 m_gen;
    
    // 生成模式权重（单个:小组:大组）
    std::vector<int> m_spawnWeights{1, 2, 3};

    std::vector<sf::Sprite> m_sprites;  // 存储生成的障碍物和道具精灵

    ObstacleModel m_obstacleModel;
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
};