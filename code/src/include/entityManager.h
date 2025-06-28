#pragma once

#include <map>
#include <SFML/Graphics.hpp>
#include "assetManager.h"

enum class EntityType {
    water
};

class EntityManager { 
public:
    static sf::Sprite& getSprite(const EntityType& type, const Textures& texture);

private:

private:
    static std::map<EntityType, sf::Sprite> m_sprites;  // 存储实体类型与对应的精灵
};
