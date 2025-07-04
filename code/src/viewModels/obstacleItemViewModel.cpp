#include "obstacleItemViewModel.h"

ObstacleItemViewModel::ObstacleItemViewModel(std::shared_ptr<SpriteViewModel> spriteVM) 
    : m_gen(m_rd()),
      m_spriteViewModel(spriteVM) {
    initialize();
    
    // 🔔 订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, 
                                std::shared_ptr<INotificationObserver>(this, [](INotificationObserver*){}));
}

void ObstacleItemViewModel::update(const float& dt, const sf::Vector2f& playerVelocity, const bool isSpawn) {
    updatePosition(playerVelocity);

    if (m_spawnClock.getElapsedTime().asSeconds() < m_spawnInterval || !isSpawn) {
        return;
    }
    
    m_spawnClock.restart();
    
    // 根据权重随机选择生成模式
    std::discrete_distribution<> modeDist(m_spawnWeights.begin(), m_spawnWeights.end());
    int mode = modeDist(m_gen);
    
    spawnSingle();
    // switch (mode) {
    //     case 0:  // 单个障碍物
    //         spawnSingle();
    //         break;
    //     case 1:  // 小组障碍物
    //     case 2:  // 大组障碍物
    //         // spawnGroup();
    //         break;
    // }
}

void ObstacleItemViewModel::initialize() {
    // 初始化障碍物组模式
    m_patterns.clear();

    createClusterPattern();
}

void ObstacleItemViewModel::updatePosition(const sf::Vector2f& playerVelocity) {
    for (auto& sprite : m_sprites) {
        // 更新障碍物位置
        sprite.move({-playerVelocity.x * Config::Game::PARALLAX_FACTOR,
                     -playerVelocity.y * Config::Game::PARALLAX_FACTOR});
    }

    // 移除超出屏幕的障碍物
    auto it = std::remove_if(m_sprites.begin(), m_sprites.end(),
        [](const sf::Sprite& sprite) {
            return sprite.getPosition().y < -sprite.getGlobalBounds().size.y;
        }
    );
    m_sprites.erase(it, m_sprites.end());
}

void ObstacleItemViewModel::spawnSingle() {
    std::uniform_int_distribution<> xDist(0, Config::Window::RENDER_SIZE.x);
    int x = xDist(m_gen);
    int y = Config::Window::RENDER_SIZE.y + 200;
    
    TextureType textureType = getRandomObstacleTexture(ObstacleType::any);
    SpriteType entityType = static_cast<SpriteType>(
        static_cast<int>(SpriteType::wood_1) + 
        (static_cast<int>(textureType) - static_cast<int>(TextureType::wood_1))
    );
    
    sf::Sprite obstacleSprite = m_spriteViewModel->getNewSprite(entityType, textureType);
    m_spriteViewModel->setSprite(
        obstacleSprite,
        sf::Color::White,
        {static_cast<float>(x), static_cast<float>(y)},
        Config::Player::PLAYER_SCALE
    );
    m_sprites.push_back(obstacleSprite);
}

void ObstacleItemViewModel::spawnGroup() {
    // 生成障碍物组的逻辑
    // 例如：从预定义模式中随机选择一个模式，生成对应位置的障碍物
}

void ObstacleItemViewModel::createClusterPattern() {
    Pattern pattern;
    pattern.positions = {
        {0, 0}, {12, 64}, {76, 32}, {128, -12}, {128, 96}
    };
    pattern.items = {
        ObstacleType::stone, ObstacleType::stone, ObstacleType::wood, ObstacleType::stone, ObstacleType::stone
    };
    pattern.width = 128;
    pattern.height = 108;
    m_patterns.push_back(pattern);
}

TextureType ObstacleItemViewModel::getRandomObstacleTexture(ObstacleType type) {
    switch (type) {
        case ObstacleType::wood: {
            std::uniform_int_distribution<> woodDist(0, m_obstacleModel.getWoodCount() - 1);
            int randomWood = woodDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::wood_1) + randomWood);
        }
        case ObstacleType::stone: {
            std::uniform_int_distribution<> stoneDist(0, m_obstacleModel.getStoneCount() - 1);
            int randomStone = stoneDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::stone_1) + randomStone);
        }
        case ObstacleType::boat: {
            std::uniform_int_distribution<> boatDist(0, m_obstacleModel.getBoatCount() - 1);
            int randomBoat = boatDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::boat_1) + randomBoat);
        }
        case ObstacleType::beach: {
            std::uniform_int_distribution<> beachDist(0, m_obstacleModel.getBeachCount() - 1);
            int randomBeach = beachDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::beach_1) + randomBeach);
        }
        case ObstacleType::any:
        default: {
            // 从所有类型中随机选择
            std::uniform_int_distribution<> typeDist(0, m_obstacleModel.getObstacleCount() - 1);
            int randomType = typeDist(m_gen);
            return static_cast<TextureType>(static_cast<int>(TextureType::wood_1) + randomType);
        }
    }
}
