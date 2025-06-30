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
          m_texutreType(tType),
          m_collisionBox(m_sprite.getGlobalBounds().size) {
            m_collisionBox.setPosition(m_sprite.getPosition());
            m_collisionBox.setOrigin(m_collisionBox.getLocalBounds().size / 2.f);
          }

    virtual ~Entity() = default;

    virtual void update(const sf::Vector2f& playerVelocity) {
        float moveX = -playerVelocity.x * 0.1;
        float moveY = -playerVelocity.y * 0.1;

        m_sprite.move(sf::Vector2f(moveX, moveY));
    }

    virtual bool isObstacle() { return false;}  // 是否是障碍物

    // 公共的 getter
    const sf::Sprite& getSprite() const { return m_sprite; }
    sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
    const sf::RectangleShape& getCollisionBox() const { return m_collisionBox; }  // 获取碰撞框

protected:
    sf::Sprite m_sprite;
    sf::RectangleShape m_collisionBox;  // 碰撞框
    Textures m_texutreType;
};