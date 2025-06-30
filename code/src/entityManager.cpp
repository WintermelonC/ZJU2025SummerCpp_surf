#include "entityManager.h"

std::map<EntityType, sf::Sprite> EntityManager::m_sprites;
std::vector<std::unique_ptr<Entity>> EntityManager::m_entities;

void EntityManager::loadSprites() {
    // 预加载所有实体类型的精灵
    m_sprites.emplace(EntityType::water, sf::Sprite(AssetManager::getTexture(Textures::water)));
    m_sprites.emplace(EntityType::player, sf::Sprite(AssetManager::getTexture(Textures::player_center_1)));
    for (int i = 1; i <= Config::Game::OBSTACLE_NUM; ++i) {
        EntityType entityType = static_cast<EntityType>(static_cast<int>(EntityType::obstacle_1) + i - 1);
        Textures textureType = static_cast<Textures>(static_cast<int>(Textures::obstacle_1) + i - 1);
        m_sprites.emplace(entityType, sf::Sprite(AssetManager::getTexture(textureType)));
    }
}

sf::Sprite& EntityManager::getSprite(const EntityType& type) {
    auto it = m_sprites.find(type);
    if (it != m_sprites.end()) {
        return it -> second;
    } else {
        throw std::runtime_error("Sprite for the given entity type does not exist");
    }
}

void EntityManager::setSprite(
        const EntityType& type,
        const sf::Vector2f& position,
        const sf::Vector2f& scale,
        const bool& ifCenter,
        const sf::Vector2f& origin
    ) {
    sf::Sprite& sprite = getSprite(type);
    sprite.setPosition(position);
    sprite.setScale(scale);
    if (ifCenter) {
        sprite.setOrigin({sprite.getLocalBounds().size / 2.f});
    } else {
        sprite.setOrigin(origin);
    }
}

void EntityManager::setSprite(
        sf::Sprite& sprite,
        const sf::Color color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale,
        const bool& ifCenter
    ) {
    sprite.setColor(color);
    sprite.setPosition(position);
    sprite.setScale(scale);
    if( ifCenter) {
        sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    }
}

void EntityManager::setSpriteTexture(const EntityType& type, const Textures& texture) {
    sf::Sprite& sprite = getSprite(type);
    sprite.setTexture(AssetManager::getTexture(texture), true);  // true 表示重置纹理矩形
}

void EntityManager::setSpriteTexture(sf::Sprite& sprite, const Textures& texture) {
    sprite.setTexture(AssetManager::getTexture(texture), true);  // true 表示重置纹理矩形
}

void EntityManager::updateEntities(const sf::Vector2f& playerVelocity) {
    for (auto& entity : m_entities) {
        entity->update(playerVelocity);  // 更新每个实体的状态
    }

    // 清除屏幕外的实体
    auto it = std::remove_if(m_entities.begin(), m_entities.end(),
        [](const auto& entity) {
            // 这个 Lambda 函数是移除条件：
            // 如果障碍物的Y坐标大于屏幕高度加上一个缓冲值，就返回 true
            // 返回 true 意味着 "这个元素应该被移除"
            return entity->getPosition().y < (-100);
        }
    );
    m_entities.erase(it, m_entities.end());  // 从 m_entities 中移除所有满足条件的元素
}