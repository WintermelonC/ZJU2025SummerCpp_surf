#include "common/spriteManager.h"

std::map<SpriteType, sf::Sprite> SpriteManager::m_sprites;

void SpriteManager::loadSprites() {
    // 预加载所有实体类型的精灵
    m_sprites.emplace(SpriteType::water, sf::Sprite(AssetManager::getTexture(Textures::water)));
    m_sprites.emplace(SpriteType::player, sf::Sprite(AssetManager::getTexture(Textures::player_center_1)));
}

sf::Sprite& SpriteManager::getSprite(const SpriteType& type) {
    auto it = m_sprites.find(type);
    if (it != m_sprites.end()) {
        return it -> second;
    } else {
        throw std::runtime_error("Sprite for the given entity type does not exist");
    }
}

void SpriteManager::setSprite(
        const SpriteType& type,
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

void SpriteManager::setSpriteTexture(const SpriteType& type, const Textures& texture) {
    sf::Sprite& sprite = getSprite(type);
    sprite.setTexture(AssetManager::getTexture(texture), true);  // true 表示重置纹理矩形
}
