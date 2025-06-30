#pragma once

#include "entity.h"
#include "entityManager.h"

class Obstacle : public Entity {
public:
    explicit Obstacle(sf::Sprite sprite, Textures tType)
        : Entity(std::move(sprite), tType) {}

    explicit Obstacle(const Obstacle& other)
        : Entity(other.m_sprite, other.m_texutreType) {}


    ~Obstacle() = default;
};