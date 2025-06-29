#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include "assetManager.h"

enum class EntityType {
    water,
    player
};

class EntityManager { 
public:
    static void loadSprites();
    
    static sf::Sprite& getSprite(const EntityType& type);

    static void setSprite(
        const EntityType& type,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true,
        const sf::Vector2f& origin = {0.0f, 0.0f}
    );

    static void setSpriteTexture(const EntityType& type, const Textures& texture);

private:

private:
    static std::map<EntityType, sf::Sprite> m_sprites;  // 存储实体类型与对应的精灵
};
