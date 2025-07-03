#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include "../viewModels/textureViewModel.h"

enum class EntityType {
    water,
    player
};

struct Entity {
    EntityType type;
    sf::Texture texture;
    sf::Vector2f position;
    sf::Vector2f origin;
    sf::Vector2f scale;
};

class SurfMap {
public:
    void clear() { entities.clear(); }
    void addEntity(const Entity& entity) { entities.push_back(entity); }
    void removeEntity(const EntityType& type) {
        entities.erase(std::remove_if(entities.begin(), entities.end(),
            [&type](const Entity& entity) { return entity.type == type; }), entities.end());
    }

    size_t getSize() const { return entities.size(); }
    const Entity& getEntity(size_t index) const { return entities[index]; }

    void setPosition(const EntityType& type, const sf::Vector2f& pos) {
        for (auto& entity : entities) {
            if (entity.type == type) {
                entity.position = pos;
                break;
            }
        }
    }
    void setTexture(const EntityType& type, const std::unique_ptr<sf::Texture>* texture) {
        for (auto& entity : entities) {
            if (entity.type == type) {
                entity.texture = *texture->get();
            }
        }
    }

private:
    std::vector<Entity> entities;
};