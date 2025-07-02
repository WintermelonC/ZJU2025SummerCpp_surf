#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include "common/assetManager.h"

enum class SpriteType {
    water,
    player
};

class SpriteManager { 
public:
    static void loadSprites();
    
    static sf::Sprite& getSprite(const SpriteType& type);

    static void setSprite(
        const SpriteType& type,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true,
        const sf::Vector2f& origin = {0.0f, 0.0f}
    );

    static void setSpriteTexture(const SpriteType& type, const Textures& texture);

private:

private:
    static std::map<SpriteType, sf::Sprite> m_sprites;  // 存储实体类型与对应的精灵
};
