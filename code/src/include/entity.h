#pragma once

#include <SFML/Graphics.hpp>
#include "entityManager.h"
#include "utils.h"
#include "assetManager.h"
#include "config.h"


class Entity {
public:
    explicit Entity(sf::Sprite sprite, Textures tType)
        : m_sprite(std::move(sprite)), 
          m_texutreType(tType) {}

    virtual ~Entity() = default;

    virtual void update(const sf::Vector2f& playerVelocity) {
        float moveX = -playerVelocity.x * 0.1;
        float moveY = -playerVelocity.y * 0.1;

        m_sprite.move(sf::Vector2f(moveX, moveY));
    }

    // 公共的 getter
    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::Vector2f getPosition() const { return m_sprite.getPosition(); }

protected:
    sf::Sprite m_sprite;
    Textures m_texutreType;
};