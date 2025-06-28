#include "entityManager.h"

sf::Sprite& EntityManager::getSprite(const EntityType& type, const Textures& texture) {
    auto it = m_sprites.find(type);
    if (it == m_sprites.end()) {
        // 不存在则创建
        auto [insertedIt, success] = m_sprites.emplace(type, sf::Sprite(AssetManager::getTexture(texture)));
        if (!success) {
            throw std::runtime_error("Failed to create sprite for the given entity type");
        }
        return insertedIt -> second;
    }
    return it -> second;
}
