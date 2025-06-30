#pragma once

#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "assetManager.h"
#include "entity.h"
#include "obstacle.h"
#include "renderSystem.h"
#include "config.h"

enum class EntityType {
    water,
    player,
    obstacle_1, obstacle_2, obstacle_3,
    obstacle_4, obstacle_5, obstacle_6,
    obstacle_7, obstacle_8, obstacle_9,
};

class EntityManager { 
public:
    static void loadSprites();
    
    static sf::Sprite& getSprite(const EntityType& type);

    static sf::Sprite getRawSprite(const EntityType& type) {
        return getSprite(type);
    }

    static void setSprite(
        const EntityType& type,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true,
        const sf::Vector2f& origin = {0.0f, 0.0f}
    );

    static void setSprite(
        sf::Sprite& sprite,
        const sf::Color color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true
    );

    static void setSpriteTexture(const EntityType& type, const Textures& texture);

    static void setSpriteTexture(sf::Sprite& sprite, const Textures& texture);

    static void pushNewEntity(const Entity& entity) {
        m_entities.emplace_back(std::make_unique<Entity>(entity));
        #ifdef DEBUG
        std::cout << "EntityManager: EntityNumber = " << m_entities.size() << std::endl;
        #endif  // DEBUG
    }

    static void updateEntities(const sf::Vector2f& playerVelocity);



private:
    static std::map<EntityType, sf::Sprite> m_sprites;  // 存储实体类型与对应的精灵

    static std::vector<std::unique_ptr<Entity>> m_entities;  // 存储所有实体对象

    friend class RenderSystem;
};
