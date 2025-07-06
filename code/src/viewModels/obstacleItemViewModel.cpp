#include "obstacleItemViewModel.h"

ObstacleItemViewModel::ObstacleItemViewModel(std::shared_ptr<TextureManager> textureVM) 
    : m_gen(m_rd()),
      m_textureManager(textureVM) {
    initialize();   
}

void ObstacleItemViewModel::subscribeToNotifications() {
    // 订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
}

void ObstacleItemViewModel::update(const float& dt) {
    if (m_gameState && *m_gameState != Config::GameState::playing) {
        return; // 如果游戏状态不是正在进行，则不更新障碍物
    }
    updatePosition();

    if (m_spawnClock.getElapsedTime().asSeconds() < m_spawnInterval || *m_playerState == Config::PlayerState::stop || m_playerVelocity->y <= 20) {
        return;
    }
    
    m_spawnClock.restart();
    
    spawnGroup();
}

void ObstacleItemViewModel::initialize() {
    // 初始化障碍物组模式
    m_patterns.clear();
    m_spriteEntityPairs.clear();
    m_obstacleItemSprites.clear();

    createClusterPattern();
    createTunnelPattern();
    createPowerPattern();
    createHeartPattern();
    createSinglePattern();
    
    // 确保精灵列表是最新的
    updateObstacleItemSprites();
    updateEntityBounds();
}

void ObstacleItemViewModel::updatePosition() {
    sf::Vector2f movement = {
        -m_playerVelocity->x * Config::Game::PARALLAX_FACTOR,
        -m_playerVelocity->y * Config::Game::PARALLAX_FACTOR
    };
    
    // 更新障碍物位置
    for (auto& pair : m_spriteEntityPairs) {
        pair.first.move(movement);
        // EntityModel在这里不需要更新，只用于类型信息
    }

    // 移除超出屏幕的障碍物
    auto it = std::remove_if(m_spriteEntityPairs.begin(), m_spriteEntityPairs.end(),
        [](const std::pair<sf::Sprite, std::shared_ptr<EntityModel>>& pair) {
            return pair.first.getPosition().y < -pair.first.getGlobalBounds().size.y;
        }
    );
    m_spriteEntityPairs.erase(it, m_spriteEntityPairs.end());
    
    // 更新独立的精灵列表
    updateObstacleItemSprites();
    updateEntityBounds();
}

bool ObstacleItemViewModel::spawnGroup() {
    if (m_patterns.empty()) {
        return false;
    }
    
    // 随机选择一个障碍物组模式
    std::uniform_int_distribution<> patternDist(0, m_patterns.size() - 1);
    const auto& pattern = m_patterns[patternDist(m_gen)];
    
    // 计算生成位置（确保整个组都在屏幕范围内）
    std::uniform_real_distribution<float> xDist(
        pattern.width / 2.f, 
        Config::Window::RENDER_SIZE.x - pattern.width / 2.f
    );
    
    // 尝试找到不碰撞的位置
    const int MAX_ATTEMPTS = 5;
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        float centerX = xDist(m_gen);
        float centerY = Config::Window::RENDER_SIZE.y * 1.5f;
        sf::Vector2f centerPos(centerX, centerY);
        
        // 检查碰撞
        sf::FloatRect patternBounds = getPatternBounds(pattern, centerPos);
        if (!checkCollision(patternBounds)) {
            // 无碰撞，生成障碍物组
            // 生成障碍物组中的每个障碍物
            for (size_t i = 0; i < pattern.positions.size(); i++) {
                sf::Vector2f worldPos = {
                    centerX + pattern.positions[i].x,
                    centerY + pattern.positions[i].y
                };
                
                auto spriteEntityPair = createSpriteEntityPair(pattern.items[i], worldPos);
                m_spriteEntityPairs.push_back(spriteEntityPair);
            }
            updateObstacleItemSprites(); // 更新独立的精灵列表
            updateEntityBounds();
            return true;
        }
    }
    
    // 所有尝试都失败，无法找到无碰撞的位置
    return false;
}

void ObstacleItemViewModel::createClusterPattern() {
    Pattern pattern;
    // 1
    pattern.positions = {
        {0, 0}, {m_stoneSize.x * 0.5f, -m_stoneSize.y}, {m_stoneSize.x, -m_stoneSize.y * 0.5f}, 
        {m_stoneSize.x * 2.0f, -m_stoneSize.y * 0.75f}, {m_stoneSize.x * 1.75f, m_stoneSize.y * 0.5f}
    };
    pattern.items = {
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood), 
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone)
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
    // 2
    pattern.positions = {
        {0, 0}, {m_seaweedSize.x, 0}, {m_seaweedSize.x * 0.5f, m_seaweedSize.y},
        {m_seaweedSize.x, m_seaweedSize.y * 2.f}, {m_seaweedSize.x * 2.f, m_seaweedSize.y * 2.f}, {m_seaweedSize.x * 3.f, m_seaweedSize.y * 2.f},
        {m_seaweedSize.x * 0.5f, -m_LBeachSize.y}, {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x, m_seaweedSize.y}, {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x, 0},
        {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x + m_bridgeSize.x, 0}, {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x + m_bridgeSize.x, m_seaweedSize.y},
        {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x + m_bridgeSize.x * 1.5f, -m_seaweedSize.y * 1.5f}, {m_seaweedSize.x * 0.5f + m_MMBridgeSize.x + m_bridgeSize.x, -m_seaweedSize.y}
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::mm_bridge),
        SpawnItem(ObstacleType::seaweed),  SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed),
        SpawnItem(ObstacleType::mm_beach), SpawnItem(ObstacleType::bridge), SpawnItem(ObstacleType::bridge),
        SpawnItem(ObstacleType::seaweed),  SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::beach),
        SpawnItem(ObstacleType::beach)
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
    // 3
    pattern.positions = {
        {0, 0}, {m_seaweedSize.x, 0}, {m_seaweedSize.x * 2.f, 0},
        {0, -m_seaweedSize.y}, {m_seaweedSize.x * 2.f, -m_seaweedSize.y}
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed),
        SpawnItem(ObstacleType::stone),  SpawnItem(ObstacleType::wood),
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
    // 4

}

void ObstacleItemViewModel::createTunnelPattern() {
    Pattern pattern;
    pattern.positions = {
        {0, 0}, {96, 0}, {192, 0}, {192, -96}, {288, -96}, {384, -96},
        {192, -96}, {192, 48}, {-24, 96}, {64, 128}, {320, 0}, {352, 48},
        {288, 192}, {320, 288}, {192, 192}, {144, 240}, {192, 288}, 
        {576, 192}, {576, 288}, {624, 96}, {624, 144}, {624, 288},
        {672, 288}, {672, 144}, {704, 384}, {800, 288}, {800, 144},
        {832, -32}, {928, 32}
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed),
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::stone), 
        SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone),
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::boat),
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood),
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::stone), 
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed),
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone)
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    // m_patterns.push_back(pattern);
}

void ObstacleItemViewModel::createPowerPattern() {
    Pattern pattern;
    pattern.positions = {
        {0, 0}, {m_seaweedSize.x, 0}, {m_seaweedSize.x * 2.f, 0},
        {-m_seaweedSize.x * 2.f, m_seaweedSize.y}, {-m_seaweedSize.x, m_seaweedSize.y}, {0, 0}, 
        {m_seaweedSize.x, m_seaweedSize.y}, {-m_seaweedSize.x * 0.75f, m_seaweedSize.y * 2.f}, {m_seaweedSize.x * 0.25f, m_seaweedSize.y * 1.75f},
        {-m_seaweedSize.x * 1.75f, m_seaweedSize.y * 3.f}, {-m_seaweedSize.x * 2.25f, m_seaweedSize.y * 4.f}, {-m_seaweedSize.x * 0.5f, m_seaweedSize.y * 3.5f},
        {m_seaweedSize.x * 0.5f, m_seaweedSize.y * 5.f}, {0, m_seaweedSize.y * 6.f}, {m_seaweedSize.x, m_seaweedSize.y * 4.f},
        {m_seaweedSize.x * 2.f, m_seaweedSize.y * 3.f}, {m_seaweedSize.x * 3.f, m_seaweedSize.y * 5.f}, {m_seaweedSize.x * 4.5f, m_seaweedSize.y * 0.5f},
        {m_seaweedSize.x * 4.5f, m_seaweedSize.y * 1.5f}, {m_seaweedSize.x * 3.5f + m_MMBridgeSize.x, m_seaweedSize.y * 2.f},
        {m_seaweedSize.x * 5.5f, m_seaweedSize.y * 0.5f}, {m_seaweedSize.x * 3.5f + m_MMBridgeSize.x + m_seaweedSize.x, m_seaweedSize.y * 3.f}, {m_seaweedSize.x * 3.5f + m_MMBridgeSize.x * 0.75f, m_seaweedSize.y * 4.f},
        {m_seaweedSize.x * 3.5f + m_MMBridgeSize.x + m_seaweedSize.x, m_seaweedSize.y * 4.f}
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::bridge), SpawnItem(ObstacleType::m_bridge),
        SpawnItem(ObstacleType::bridge), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::stone), 
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone), SpawnItem(ItemType::power),
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::bridge), 
        SpawnItem(ObstacleType::m_bridge), SpawnItem(ObstacleType::bridge), SpawnItem(ObstacleType::wood), 
        SpawnItem(ObstacleType::mm_bridge), SpawnItem(ObstacleType::m_bridge), 
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::wood),
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
}

void ObstacleItemViewModel::createHeartPattern() {
    Pattern pattern;
    pattern.positions = {
        {0, 0}, {-m_seaweedSize.x, -m_seaweedSize.y}, {-m_seaweedSize.x * 0.5f, m_seaweedSize.y * 0.5f},
        {-m_seaweedSize.x * 3.f, m_seaweedSize.y * 0.5f}, {-m_seaweedSize.x - m_MMBeachSize.x, m_seaweedSize.y * 1.5f}, {-m_seaweedSize.x * 1.25f - m_MMBeachSize.x, m_seaweedSize.y * 1.5f + m_MMBeachSize.y * 0.75f},
        {-m_seaweedSize.x * 3.25f, m_seaweedSize.y * 0.5f + m_MMBeachSize.y}, {-m_seaweedSize.x * 2.5f, m_seaweedSize.y * 1.f + m_MMBeachSize.y}, {-m_seaweedSize.x * 1.5f, m_seaweedSize.y * 1.5f + m_LBeachSize.y},
        {m_seaweedSize.x * 0.25f, m_seaweedSize.y * 3.f}, {m_seaweedSize.x * 2.f, m_seaweedSize.y * 3.f}, {m_seaweedSize.x * 1.5f, m_seaweedSize.y},
        {m_seaweedSize.x * 3.75f, m_seaweedSize.y * 1.f}, {m_seaweedSize.x * 3.5f, m_seaweedSize.y * 1.f}, 
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::m_beach), SpawnItem(ObstacleType::beach), 
        SpawnItem(ObstacleType::m_beach), SpawnItem(ObstacleType::mm_beach), SpawnItem(ObstacleType::beach),
        SpawnItem(ObstacleType::m_beach), SpawnItem(ObstacleType::m_beach), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ItemType::heart), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::l_beach), 
        SpawnItem(ObstacleType::beach), SpawnItem(ObstacleType::m_beach),
    };
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
}

void ObstacleItemViewModel::createSinglePattern() {
    Pattern pattern;
    // 1
    pattern.positions = {{0, 0}};
    pattern.items = {SpawnItem(ObstacleType::mm_beach)};
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
    // 2
    pattern.positions = {{0, 0}};
    pattern.items = {SpawnItem(ObstacleType::boat)};
    setPatternPosition(pattern);
    setPatternSize(pattern);
    m_patterns.push_back(pattern);
}

TextureType ObstacleItemViewModel::getRandomObstacleTexture(ObstacleType type) {
    switch (type) {
        case ObstacleType::wood: {
            std::uniform_int_distribution<> woodDist(0, ObstacleModel::getWoodCount() - 1);
            int randomWood = woodDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::wood_1) + randomWood);
        }
        case ObstacleType::stone: {
            std::uniform_int_distribution<> stoneDist(0, ObstacleModel::getStoneCount() - 1);
            int randomStone = stoneDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::stone_1) + randomStone);
        }
        case ObstacleType::boat: {
            std::uniform_int_distribution<> boatDist(0, ObstacleModel::getBoatCount() - 1);
            int randomBoat = boatDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::boat_1) + randomBoat);
        }
        case ObstacleType::beach: {
            std::uniform_int_distribution<> beachDist(0, ObstacleModel::getBeachCount() - 1);
            int randomBeach = beachDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::beach_1) + randomBeach);
        }
        case ObstacleType::seaweed: {
            std::uniform_int_distribution<> seaweedDist(0, ObstacleModel::getSeaweedCount() - 1);
            int randomSeaweed = seaweedDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::seaweed_11) + randomSeaweed);
        }
        case ObstacleType::ripple: {
            std::uniform_int_distribution<> rippleDist(0, ObstacleModel::getRippleCount() - 1);
            int randomRipple = rippleDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::ripple_11) + randomRipple);
        }
        case ObstacleType::bridge: {
            std::uniform_int_distribution<> bridgeDist(0, ObstacleModel::getBridgeCount() - 1);
            int randomBridge = bridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::bridge_1) + randomBridge);
        }
        case ObstacleType::sp_bridge: {
            std::uniform_int_distribution<> spBridgeDist(0, ObstacleModel::getSpBridgeCount() - 1);
            int randomSpBridge = spBridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::sp_bridge_1) + randomSpBridge);
        }
        case ObstacleType::buoy: {
            std::uniform_int_distribution<> buoyDist(0, ObstacleModel::getBuoyCount() - 1);
            int randomBuoy = buoyDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::buoy_1) + randomBuoy);
        }
        case ObstacleType::coral: {
            std::uniform_int_distribution<> coralDist(0, ObstacleModel::getCoralCount() - 1);
            int randomCoral = coralDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::coral_1) + randomCoral);
        }
        case ObstacleType::l_beach: {
            std::uniform_int_distribution<> lBeachDist(0, ObstacleModel::getLBeachCount() - 1);
            int randomLBeach = lBeachDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::l_beach_1) + randomLBeach);
        }
        case ObstacleType::l_bridge: {
            std::uniform_int_distribution<> lBridgeDist(0, ObstacleModel::getLBridgeCount() - 1);
            int randomLBridge = lBridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::l_bridge_1) + randomLBridge);
        }
        case ObstacleType::m_beach: {
            std::uniform_int_distribution<> mBeachDist(0, ObstacleModel::getMBeachCount() - 1);
            int randomMBeach = mBeachDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::m_beach_1) + randomMBeach);
        }
        case ObstacleType::m_boat: {
            std::uniform_int_distribution<> mBoatDist(0, ObstacleModel::getMBoatCount() - 1);
            int randomMBoat = mBoatDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::m_boat_1) + randomMBoat);
        }
        case ObstacleType::m_bridge: {
            std::uniform_int_distribution<> mBridgeDist(0, ObstacleModel::getMBridgeCount() - 1);
            int randomMBridge = mBridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::m_bridge_1) + randomMBridge);
        }
        case ObstacleType::mm_bridge: {
            std::uniform_int_distribution<> mmBridgeDist(0, ObstacleModel::getMMBridgeCount() - 1);
            int randomMMBridge = mmBridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::mm_bridge_1) + randomMMBridge);
        }
        case ObstacleType::mm_beach: {
            std::uniform_int_distribution<> mmBeachDist(0, ObstacleModel::getMMBeachCount() - 1);
            int randomMMBeach = mmBeachDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::mm_beach_1) + randomMMBeach);
        }
        case ObstacleType::s_buoy: {
            std::uniform_int_distribution<> sBuoyDist(0, ObstacleModel::getSBuoyCount() - 1);
            int randomSBuoy = sBuoyDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::s_buoy_1) + randomSBuoy);
        }
        case ObstacleType::any:
        default: {
            std::uniform_int_distribution<> dist(0, static_cast<int>(TextureType::s_buoy_1) - static_cast<int>(TextureType::wood_1));
            int randomType = dist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::wood_1) + randomType);
        }
    }
}

bool ObstacleItemViewModel::isPositionValid(const sf::Vector2f& position) {
    return position.x >= 0 && position.x <= Config::Window::RENDER_SIZE.x;
}

bool ObstacleItemViewModel::checkCollision(const sf::FloatRect& newRect) {
    // 检查是否与任何已有的边界碰撞
    for (const auto& existingBounds : m_entityBounds) {
        if (newRect.findIntersection(existingBounds)) {
            return true; // 检测到碰撞
        }
    }
    return false; // 没有碰撞
}

sf::FloatRect ObstacleItemViewModel::getPatternBounds(const Pattern& pattern, const sf::Vector2f& centerPos) {
    if (pattern.positions.empty()) {
        return sf::FloatRect({centerPos.x, centerPos.y}, {0, 0});
    }
    
    // 找到模式中所有位置的边界
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    
    for (const auto& relativePos : pattern.positions) {
        float worldX = centerPos.x + relativePos.x;
        float worldY = centerPos.y + relativePos.y;
        
        minX = std::min(minX, worldX);
        minY = std::min(minY, worldY);
        maxX = std::max(maxX, worldX);
        maxY = std::max(maxY, worldY);
    }
    
    // 添加一些边距，避免组太近
    const float MARGIN = 20.0f;
    minX -= MARGIN;
    minY -= MARGIN;
    maxX += MARGIN;
    maxY += MARGIN;
    
    return sf::FloatRect({minX, minY}, {maxX - minX, maxY - minY});
}

void ObstacleItemViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            resetObstacles();
            break;
        }
        default:
            break;
    }
}

void ObstacleItemViewModel::resetObstacles() {
    // 清空所有障碍物精灵和EntityModel对
    m_spriteEntityPairs.clear();
    
    // 清空独立的精灵列表
    m_obstacleItemSprites.clear();
    
    // 清空活动边界
    m_entityBounds.clear();
    
    // 重置生成计时器
    m_spawnClock.restart();
    
    // 重置模式相关参数
    m_patterns.clear();
    
    // 重新初始化
    initialize();
}

std::pair<sf::Sprite, std::shared_ptr<EntityModel>> ObstacleItemViewModel::createSpriteEntityPair(const SpawnItem& item, const sf::Vector2f& position) {
    std::shared_ptr<EntityModel> entityModel;
    
    switch (item.type) {
        case SpawnItem::Type::Obstacle: {
            TextureType textureType = getRandomObstacleTexture(item.obstacleType);
            sf::Sprite sprite = m_textureManager->getNewSprite(textureType);
            Utils::setSprite(
                sprite,
                sf::Color::White,
                position,
                Config::Player::PLAYER_SCALE,
                false
            );
            entityModel = createEntityModel(item.obstacleType);
            return std::make_pair(sprite, entityModel);
        }
        case SpawnItem::Type::Item: {
            // TODO: 实现道具的纹理选择逻辑
            // 暂时使用默认纹理
            TextureType textureType = getItemTexture(item.itemType);
            sf::Sprite sprite = m_textureManager->getNewSprite(textureType);
            Utils::setSprite(
                sprite,
                sf::Color::White,
                position,
                Config::Player::PLAYER_SCALE,
                false
            );
            entityModel = createEntityModel(item.itemType);
            return std::make_pair(sprite, entityModel);
        }
    }
    
    // 默认情况，不应该到达这里
    sf::Sprite defaultSprite = m_textureManager->getNewSprite(TextureType::stone_1);
    entityModel = createEntityModel(ObstacleType::stone);
    return std::make_pair(defaultSprite, entityModel);
}

std::shared_ptr<EntityModel> ObstacleItemViewModel::createEntityModel(ObstacleType obstacleType) {
    return std::make_shared<ObstacleModel>(obstacleType);
}

std::shared_ptr<EntityModel> ObstacleItemViewModel::createEntityModel(ItemType itemType) {
    return std::make_shared<ItemModel>(itemType);
}

bool ObstacleItemViewModel::checkCollisionWithPlayer(const sf::Sprite& playerSprite) {
    if (m_gameState && *m_gameState != Config::GameState::playing) {
        return false;
    }
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    sf::Vector2f center = playerBounds.getCenter();

    sf::Vector2f newSize = {
        playerBounds.size.x * Config::Game::PLAYER_SIZE_X_COLLISION_SCALE,
        playerBounds.size.y * Config::Game::PLAYER_SIZE_Y_COLLISION_SCALE
    };

    sf::Vector2f newPosition = {
        center.x - newSize.x / 2.f + Config::Game::PLAYER_POSITION_X_OFFSET,
        center.y - newSize.y / 2.f + Config::Game::PLAYER_POSITION_Y_OFFSET
    };

    playerBounds = sf::FloatRect(newPosition, newSize);

    auto& notificationCenter = NotificationCenter::getInstance();
    
    for (auto it = m_spriteEntityPairs.begin(); it != m_spriteEntityPairs.end(); it++) {
        // 获取当前精灵的边界
        auto that = m_entityBounds.begin() + (it - m_spriteEntityPairs.begin());
        sf::FloatRect spriteBounds = *that;
        
        // 使用findIntersection进行精确碰撞检测
        if (playerBounds.findIntersection(spriteBounds)) {
            if (it->second->isObstacle()) {
                // std::cout << "Obstacle collision detected with type: " << static_cast<int>(it->second->getObstacleType()) << std::endl;
                // 障碍物碰撞处理
                ObstacleType obstacleType = it->second->getObstacleType();
                
                if (obstacleType == ObstacleType::seaweed) {
                    // 海草减速效果
                    SlowCollisionData slowData;
                    notificationCenter.postNotification(slowData);
                } else {
                    // 其他障碍物造成伤害
                    DamageCollisionData damageData;
                    notificationCenter.postNotification(damageData);
                }
                return true;
            }
            else if (it->second->isItem()) {
                // 道具碰撞处理 - 无论是否无敌都可以收集道具
                ItemType itemType = it->second->getItemType();
                
                switch (itemType) {
                    case ItemType::power: {
                        PowerCollisionData speedData;
                        notificationCenter.postNotification(speedData);
                        break;
                    }
                    case ItemType::heart: {
                        HealthCollisionData healthData;
                        notificationCenter.postNotification(healthData);
                        break;
                    }
                    default:
                        break;
                }
                
                // 移除已收集的道具
                it = m_spriteEntityPairs.erase(it);
                updateObstacleItemSprites();
                updateEntityBounds();
                return true;
            }
        }
    }
    return false;
}

void ObstacleItemViewModel::updateObstacleItemSprites() {
    // 清空当前的精灵列表
    m_obstacleItemSprites.clear();
    
    // 预分配空间以提高性能
    m_obstacleItemSprites.reserve(m_spriteEntityPairs.size());
    
    // 从配对中提取所有精灵
    for (const auto& pair : m_spriteEntityPairs) {
        m_obstacleItemSprites.push_back(pair.first);
    }
}

void ObstacleItemViewModel::updateEntityBounds() {
    // 清空当前的entityBounds
    m_entityBounds.clear();

    // 预分配空间以提高性能
    m_entityBounds.reserve(m_spriteEntityPairs.size());

    // 从配对中提取所有EntityModel的边界
    for (const auto& pair : m_spriteEntityPairs) {
        sf::FloatRect bounds = pair.first.getGlobalBounds();
        EntityModel *entityModel = pair.second.get();

        sf::Vector2f center = bounds.getCenter();

        sf::Vector2f newSize = {
            bounds.size.x * Config::Game::PLAYER_SIZE_X_COLLISION_SCALE,
            bounds.size.y * Config::Game::PLAYER_SIZE_Y_COLLISION_SCALE
        };

        sf::Vector2f newPosition = {
            center.x - newSize.x / 2.f,
            center.y - newSize.y / 2.f
        };

        if (entityModel->isObstacle()) {
            ObstacleType obstacleType = entityModel->getObstacleType();
            switch(obstacleType) {
                case ObstacleType::stone: {
                    setRecNewSize(newSize, {1.2f, 1.f});
                    setRecNewPos(newPosition, center, newSize, {0, 20.f});
                    break;
                }
                case ObstacleType::wood: {
                    setRecNewSize(newSize, {0.9f, 1.f});
                    setRecNewPos(newPosition, center, newSize, {0, 20.f});
                    break;
                }
                case ObstacleType::seaweed: {
                    newSize = bounds.size;
                    setRecNewSize(newSize, {0.8f, 0.8f});
                    setRecNewPos(newPosition, center, newSize);
                    break;
                }
                case ObstacleType::boat: {
                    setRecNewSize(newSize, {1.2f, 1.f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 20.f});
                    break;
                }
                case ObstacleType::beach: {
                    setRecNewSize(newSize, {1.1f, 1.f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 20.f});
                    break;
                }
                case ObstacleType::ripple: break;
                case ObstacleType::bridge: {
                    setRecNewSize(newSize, {2.5f, 2.f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 20.f});
                    break;
                }
                case ObstacleType::sp_bridge: break;
                case ObstacleType::buoy: break;
                case ObstacleType::coral: break;
                case ObstacleType::l_beach: break;
                case ObstacleType::l_bridge: break;
                case ObstacleType::m_beach: {
                    setRecNewSize(newSize, {1.5f, 0.8f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 40.f});
                    break;
                }
                case ObstacleType::m_boat: break;
                case ObstacleType::m_bridge: {
                    setRecNewSize(newSize, {1.5f, 1.f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 40.f});
                    break;
                }
                case ObstacleType::mm_bridge: break;
                case ObstacleType::mm_beach: {
                    setRecNewSize(newSize, {3.f, 1.5f});
                    setRecNewPos(newPosition, center, newSize, {0.f, 40.f});
                    break;
                }
                case ObstacleType::s_buoy: break;
            }
        } else if (entityModel->isItem()) {
            ItemType itemType = entityModel->getItemType();
            switch (itemType) {
                case ItemType::power: {
                    newSize = bounds.size;
                    setRecNewSize(newSize, {0.8f, 0.8f});
                    setRecNewPos(newPosition, center, newSize);
                    break;
                }
                case ItemType::heart: {
                    newSize = bounds.size;
                    setRecNewSize(newSize, {0.8f, 0.8f});
                    setRecNewPos(newPosition, center, newSize);
                    break;
                }
                default: {
                    newSize = bounds.size;
                    setRecNewSize(newSize, {0.8f, 0.8f});
                    setRecNewPos(newPosition, center, newSize);
                    break;
                }
            }
        }
        bounds = sf::FloatRect(newPosition, newSize);
        m_entityBounds.push_back(bounds);
    }
}

void ObstacleItemViewModel::setPatternSize(Pattern& pattern) {
    if (pattern.positions.empty()) {
        return;
    }
    
    int maxWidth = 0;
    int minWidth = 0;
    int maxHeight = 0;
    int minHeight = 0;
    
    for (const auto& pos : pattern.positions) {
        maxWidth = std::max(maxWidth, static_cast<int>(pos.x));
        minWidth = std::min(minWidth, static_cast<int>(pos.x));
        maxHeight = std::max(maxHeight, static_cast<int>(pos.y));
        minHeight = std::min(minHeight, static_cast<int>(pos.y));
    }
    
    pattern.width = maxWidth - minWidth;
    pattern.height = maxHeight - minHeight;
}

void ObstacleItemViewModel::setPatternPosition(Pattern& pattern) {
    if (pattern.positions.empty()) {
        return;
    }
    
    for (auto& pos : pattern.positions) {
        pos.x *= Config::Player::PLAYER_SCALE.x;
        pos.y *= Config::Player::PLAYER_SCALE.y;
    }
}

TextureType ObstacleItemViewModel::getItemTexture(ItemType type) {
    switch (type) {
        case ItemType::power:
            return TextureType::power_item_1;
        case ItemType::heart:
            return TextureType::heart_item_1;
        default:
            return TextureType::power_item_1;
    }
}

void ObstacleItemViewModel::setRecNewPos(sf::Vector2f& Pos, const sf::Vector2f& center, const sf::Vector2f& size, const sf::Vector2f& offset) {
    Pos.x = center.x - size.x / 2.f + offset.x;
    Pos.y = center.y - size.y / 2.f + offset.y;
}

void ObstacleItemViewModel::setRecNewSize(sf::Vector2f& size, const sf::Vector2f& scale) {
    size.x *= scale.x;
    size.y *= scale.y;
}