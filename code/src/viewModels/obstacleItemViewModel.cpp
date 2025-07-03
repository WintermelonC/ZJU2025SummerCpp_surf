#include "ObstacleItemViewModel.h"

ObstacleItemViewModel::ObstacleItemViewModel() 
    : m_gen(m_rd()) {
    initialize();
}

void ObstacleItemViewModel::update(const float& dt) {
    if (m_spawnClock.getElapsedTime().asSeconds() < m_spawnInterval) {
        return;
    }
    
    m_spawnClock.restart();
    
    // 根据权重随机选择生成模式
    std::discrete_distribution<> modeDist(m_spawnWeights.begin(), m_spawnWeights.end());
    int mode = modeDist(m_gen);
    
    switch (mode) {
        case 0:  // 单个障碍物
            spawnSingle();
            break;
        case 1:  // 小组障碍物
        case 2:  // 大组障碍物
            spawnGroup();
            break;
    }
}

void ObstacleItemViewModel::initialize() {
    // 初始化障碍物组模式
    m_patterns.clear();

    createClusterPattern();
}

void ObstacleItemViewModel::spawnSingle() {
    std::uniform_int_distribution<> xDist(0, Config::Window::RENDER_SIZE.x);
    int x = xDist(m_gen);
    int y = Config::Window::RENDER_SIZE.y;
    
    Textures textureType = getRandomObstacleTexture(ObstacleType::Any);
    EntityType entityType = static_cast<EntityType>(
        static_cast<int>(EntityType::wood_1) + 
        (static_cast<int>(textureType) - static_cast<int>(Textures::wood_1))
    );
    
    sf::Sprite obstacleSprite = EntityManager::getRawSprite(entityType);
    EntityManager::setSprite(
        obstacleSprite,
        sf::Color::White,
        {static_cast<float>(x), static_cast<float>(y)},
        Config::Player::PLAYER_SCALE
    );
    
    Obstacle obstacle(obstacleSprite, textureType);
    EntityManager::pushNewEntity(obstacle);
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
