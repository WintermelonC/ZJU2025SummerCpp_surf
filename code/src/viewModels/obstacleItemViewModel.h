#pragma once

#include <random>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../common/notificationCenter.h"
#include "../common/utils.h"
#include "../common/textureManager.h"
#include "../models/itemModel.h"
#include "../models/obstacleModel.h"

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

class ObstacleItemViewModel : public INotificationObserver, public std::enable_shared_from_this<ObstacleItemViewModel> {
public:
    ObstacleItemViewModel(std::shared_ptr<TextureManager> textureVM);

    // 主要更新方法
    void update(const float& dt);
    
    // 获取精灵和实体模型
    const std::vector<std::pair<sf::Sprite, std::shared_ptr<EntityModel>>>& getSpriteEntityPairs() const { 
        return m_spriteEntityPairs; 
    }

    // 获取障碍物和道具精灵
    const std::vector<sf::Sprite>& getObstacleItemSprites() const { 
        return m_obstacleItemSprites; 
    }

    const std::vector<sf::FloatRect>& getEntityBounds() const { 
        return m_entityBounds; 
    }
    
    // 碰撞检测相关
    bool checkCollisionWithPlayer(const sf::Sprite& playerSprite);
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    // 在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();

    // Setter 方法
    void setPlayerVelocity(const sf::Vector2f* playerVelocity) { m_playerVelocity = playerVelocity; }
    void setPlayerState(const Config::PlayerState* playerState) { m_playerState = playerState; }
    void setGameState(const Config::GameState* gameState) { m_gameState = gameState; }

    // 获取更新回调
    Config::ObstacleItemUpdateCallback getUpdateCommand();

private:
    // 障碍物和道具组合模式结构
    struct Pattern {
        std::vector<SpawnItem> items;      // 包含障碍物和道具的生成项
        std::vector<sf::Vector2f> positions;  // 相对位置
        float width;   // 模式宽度
        float height;  // 模式高度
    };

    // 初始化和重置方法
    void initialize();
    void resetObstacles();  // 内部重置方法
    void updatePosition();

    // 生成相关方法
    bool spawnGroup();
    
    // 创建sprite和对应EntityModel的方法
    std::pair<sf::Sprite, std::shared_ptr<EntityModel>> createSpriteEntityPair(
        const SpawnItem& item, 
        const sf::Vector2f& position
    );
    
    // 根据类型创建对应的EntityModel
    std::shared_ptr<EntityModel> createEntityModel(ObstacleType obstacleType);
    std::shared_ptr<EntityModel> createEntityModel(ItemType itemType);

    // 更新精灵列表
    void updateObstacleItemSprites();
    // 更新实体边界
    void updateEntityBounds();

    // 模式创建方法
    void createClusterPattern();  // 聚集模式
    void createTunnelPattern();  // 隧道模式
    void createPowerPattern();  // 能量道具模式
    void createHeartPattern();  // 生命道具模式
    void createSinglePattern();

    // 工具方法
    TextureType getRandomObstacleTexture(ObstacleType type);
    bool isPositionValid(const sf::Vector2f& position);
    bool checkCollision(const sf::FloatRect& newRect);
    sf::FloatRect getPatternBounds(const Pattern& pattern, const sf::Vector2f& centerPos);
    void setPatternSize(Pattern& pattern);
    void setPatternPosition(Pattern& pattern);
    TextureType getItemTexture(ItemType type);
    void setRecNewPos(sf::Vector2f& Pos, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Vector2f& offset = {0.f, 0.f});
    void setRecNewSize(sf::Vector2f& size, const sf::Vector2f& scale = {1.f, 1.f});

private:
    // 生成配置
    const float m_spawnInterval = 1.0f;  // 生成间隔时间
    const sf::Vector2f m_SBuoySize = Config::Texture::SIZE_1;
    const sf::Vector2f m_buoySize = Config::Texture::SIZE_2;
    const sf::Vector2f m_coralSize = m_buoySize;
    const sf::Vector2f m_stoneSize = m_buoySize;
    const sf::Vector2f m_woodSize = m_buoySize;
    const sf::Vector2f m_bridgeSize = m_buoySize;
    const sf::Vector2f m_boatSize = m_buoySize;
    const sf::Vector2f m_beachSize = m_buoySize;
    const sf::Vector2f m_seaweedSize = m_beachSize;
    const sf::Vector2f m_MMBridgeSize = Config::Texture::SIZE_3;
    const sf::Vector2f m_MBeachSize = Config::Texture::SIZE_4;
    const sf::Vector2f m_MBoatSize = m_MBeachSize;
    const sf::Vector2f m_MBridgeSize = m_MBeachSize;
    const sf::Vector2f m_LBeachSize = Config::Texture::SIZE_5;
    const sf::Vector2f m_LBridgeSize = Config::Texture::SIZE_6;
    const sf::Vector2f m_MMBeachSize = Config::Texture::SIZE_7;

    std::vector<int> m_spawnWeights{1, 2, 3};  // 生成模式权重（单个:小组:大组）
    
    // 数据存储
    std::vector<Pattern> m_patterns;  // 预定义的障碍物和道具组合模式
    std::vector<sf::Sprite> m_obstacleItemSprites;  // 存储精灵
    std::vector<sf::FloatRect> m_entityBounds;  // 存储实体的边界
    std::vector<std::pair<sf::Sprite, std::shared_ptr<EntityModel>>> m_spriteEntityPairs;
    
    // 时钟和随机数
    sf::Clock m_spawnClock;  // 生成时钟
    std::random_device m_rd;
    std::mt19937 m_gen;

    // 外部依赖
    std::shared_ptr<TextureManager> m_textureManager;
    const sf::Vector2f* m_playerVelocity;
    const Config::PlayerState* m_playerState;
    const Config::GameState* m_gameState;
};