#include "obstacleItemViewModel.h"

ObstacleItemViewModel::ObstacleItemViewModel(std::shared_ptr<SpriteViewModel> spriteVM) 
    : m_gen(m_rd()),
      m_spriteViewModel(spriteVM) {
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

    if (m_spawnClock.getElapsedTime().asSeconds() < m_spawnInterval || *m_playerState == PlayerState::stop || m_playerVelocity->y <= 20) {
        return;
    }
    
    m_spawnClock.restart();
    
    // 根据权重随机选择生成模式
    std::discrete_distribution<> modeDist(m_spawnWeights.begin(), m_spawnWeights.end());
    int mode = modeDist(m_gen);
    
    // 尝试生成组，如果失败（如发生碰撞）则尝试生成单个障碍物
    if (!spawnGroup()) {
        spawnSingle();
    }
    
    // 清理超出屏幕的碰撞盒
    auto it = std::remove_if(m_activeBounds.begin(), m_activeBounds.end(),
        [](const sf::FloatRect& rect) {
            return rect.position.y + rect.size.y < 0; // 如果已经完全超出屏幕顶部
        }
    );
    m_activeBounds.erase(it, m_activeBounds.end());
}

void ObstacleItemViewModel::initialize() {
    // 初始化障碍物组模式
    m_patterns.clear();
    m_spriteEntityPairs.clear();
    m_obstacleItemSprites.clear();

    createClusterPattern();
    createTunnelPattern();
    
    // 确保精灵列表是最新的
    updateObstacleItemSprites();
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

    // 更新碰撞盒位置
    for (auto& bounds : m_activeBounds) {
        bounds.position.x += movement.x;
        bounds.position.y += movement.y;
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
}

bool ObstacleItemViewModel::spawnSingle() {
    const int MAX_ATTEMPTS = 5;
    
    for (int attempt = 0; attempt < MAX_ATTEMPTS; attempt++) {
        std::uniform_int_distribution<> xDist(0, Config::Window::RENDER_SIZE.x);
        int x = xDist(m_gen);
        int y = Config::Window::RENDER_SIZE.y * 1.5f;
        
        // 随机选择障碍物类型
        ObstacleType obstacleType = ObstacleType::any;
        SpawnItem item(obstacleType);
        
        // 创建临时精灵以获取边界
        auto spriteEntityPair = createSpriteEntityPair(item, {static_cast<float>(x), static_cast<float>(y)});
        
        // 添加边距以避免紧挨着
        sf::FloatRect bounds = spriteEntityPair.first.getGlobalBounds();
        bounds.position.x -= 10;
        bounds.position.y -= 10;
        bounds.size.x += 20;
        bounds.size.y += 20;
        
        if (!checkCollision(bounds)) {
            m_activeBounds.push_back(bounds);
            m_spriteEntityPairs.push_back(spriteEntityPair);
            updateObstacleItemSprites(); // 更新独立的精灵列表
            return true;
        }
    }
    
    // 所有尝试都失败
    return false;
}

bool ObstacleItemViewModel::spawnGroup() {
    if (m_patterns.empty()) {
        return spawnSingle();
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
            m_activeBounds.push_back(patternBounds);
            
            // 生成障碍物组中的每个障碍物
            for (size_t i = 0; i < pattern.positions.size(); i++) {
                sf::Vector2f worldPos = {
                    centerX + pattern.positions[i].x,
                    centerY + pattern.positions[i].y
                };
                
                if (isPositionValid(worldPos)) {
                    // 创建 sprite 和 EntityModel 配对
                    auto spriteEntityPair = createSpriteEntityPair(pattern.items[i], worldPos);
                    m_spriteEntityPairs.push_back(spriteEntityPair);
                }
            }
            updateObstacleItemSprites(); // 更新独立的精灵列表
            return true;
        }
    }
    
    // 所有尝试都失败，无法找到无碰撞的位置
    return false;
}

void ObstacleItemViewModel::createClusterPattern() {
    Pattern pattern;
    pattern.positions = {
        {0, 0}, {12, 64}, {96, 32}, {196, -12}, {160, 96}
    };
    pattern.items = {
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::wood), 
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone)
    };
    pattern.width = 0;
    pattern.height = 0;
    for (const auto& pos : pattern.positions) {
        pattern.width = std::max(pattern.width, pos.x);
        pattern.height = std::max(pattern.height, pos.y);
    }
    m_patterns.push_back(pattern);
    
    pattern.positions = {
        {0, 0}, {96, -128}, {96, 0}, {192, 0}, {128, -48}
    };
    pattern.items = {
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::seaweed), SpawnItem(ObstacleType::bridge)
    };
    pattern.width = 0;
    pattern.height = 0;
    for (const auto& pos : pattern.positions) {
        pattern.width = std::max(pattern.width, pos.x);
        pattern.height = std::max(pattern.height, pos.y);
    }
    m_patterns.push_back(pattern);

    pattern.positions = {
        {0, 0}, {64, 64}, {96, -48}, {128, 128}, {196, 0}
    };
    pattern.items = {
        SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::stone), SpawnItem(ObstacleType::seaweed), 
        SpawnItem(ObstacleType::wood), SpawnItem(ObstacleType::stone)
    };
    pattern.width = 0;
    pattern.height = 0;
    for (const auto& pos : pattern.positions) {
        pattern.width = std::max(pattern.width, pos.x);
        pattern.height = std::max(pattern.height, pos.y);
    }
    m_patterns.push_back(pattern);
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
    pattern.width = 0;
    pattern.height = 0;
    for (const auto& pos : pattern.positions) {
        pattern.width = std::max(pattern.width, pos.x);
        pattern.height = std::max(pattern.height, pos.y);
    }
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
        case ObstacleType::bridge: {
            std::uniform_int_distribution<> bridgeDist(0, ObstacleModel::getBridgeCount() - 1);
            int randomBridge = bridgeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::bridge_1) + randomBridge);
        }
        case ObstacleType::any:
        default: {
            // 从所有类型中随机选择
            std::uniform_int_distribution<> typeDist(0, ObstacleModel::getObstacleCount() - 1);
            int randomType = typeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::wood_1) + randomType);
        }
    }
}

bool ObstacleItemViewModel::isPositionValid(const sf::Vector2f& position) {
    return position.x >= 0 && position.x <= Config::Window::RENDER_SIZE.x;
}

bool ObstacleItemViewModel::checkCollision(const sf::FloatRect& newRect) {
    // 检查是否与任何已有的边界碰撞
    for (const auto& existingBounds : m_activeBounds) {
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
    m_activeBounds.clear();
    
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
            sf::Sprite sprite = m_spriteViewModel->getNewSprite(textureType);
            m_spriteViewModel->setSprite(
                sprite,
                sf::Color::White,
                position,
                Config::Player::PLAYER_SCALE
            );
            entityModel = createEntityModel(item.obstacleType);
            return std::make_pair(sprite, entityModel);
        }
        case SpawnItem::Type::Item: {
            // TODO: 实现道具的纹理选择逻辑
            // 暂时使用默认纹理
            sf::Sprite sprite = m_spriteViewModel->getNewSprite(TextureType::heart_1);
            m_spriteViewModel->setSprite(
                sprite,
                sf::Color::White,
                position,
                Config::Player::PLAYER_SCALE
            );
            entityModel = createEntityModel(item.itemType);
            return std::make_pair(sprite, entityModel);
        }
    }
    
    // 默认情况，不应该到达这里
    sf::Sprite defaultSprite = m_spriteViewModel->getNewSprite(TextureType::stone_1);
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
    sf::FloatRect playerBounds = playerSprite.getGlobalBounds();

    sf::Vector2f center = {
        playerBounds.position.x + playerBounds.size.x / 2,
        playerBounds.position.y + playerBounds.size.y / 2
    };

    sf::Vector2f newSize = playerBounds.size * 0.66f;
    sf::Vector2f newPosition = {
        center.x - newSize.x / 2,
        center.y - newSize.y / 2
    };

    playerBounds = sf::FloatRect(newPosition, newSize);

    auto& notificationCenter = NotificationCenter::getInstance();
    
    for (auto it = m_spriteEntityPairs.begin(); it != m_spriteEntityPairs.end(); it++) {
        sf::FloatRect spriteBounds = it->first.getGlobalBounds();
        
        // 使用findIntersection进行精确碰撞检测
        if (playerBounds.findIntersection(spriteBounds)) {
            if (it->second->isObstacle()) {
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