#pragma once

#include <SFML/Graphics.hpp>
#include "utils.h"

class Entity {
public:
    explicit Entity(sf::Sprite sprite, Textures type)
        : m_sprite(std::move(sprite)), m_type(type) {}

    virtual ~Entity() = default;

    virtual void update(float dt, const sf::Vector2f& playerVelocity) = 0;

    // 公共的 getter
    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::Vector2f getPosition() const { return m_sprite.getPosition(); }

protected:
    sf::Sprite m_sprite;
    Textures m_type;
};