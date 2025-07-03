#pragma once

#include <SFML/Graphics.hpp>

class EntityModel {
public:
    EntityModel(const sf::Vector2f& pos = {0, 0}, 
                const sf::Vector2f& sz = {1, 1})
        : position(pos), 
          size(sz) {};
    virtual ~EntityModel() = default;

    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getSize() const { return size; }
    const sf::RectangleShape& getCollisionBox() const { return m_collisionBox; }

    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setSize(const sf::Vector2f& sz) { size = sz; }
    void setCollisionBox(const sf::Vector2f& pos, const sf::Vector2f& origin, const sf::Vector2f& sz, const sf::Angle& angle, const sf::Color& color = sf::Color(255, 255, 255, 100)) {
        m_collisionBox.setOrigin(origin);
        m_collisionBox.setPosition(pos);
        m_collisionBox.setSize(sz);
        m_collisionBox.setFillColor(color);
        m_collisionBox.setOutlineColor(sf::Color::White);
        m_collisionBox.setRotation(angle);
    }

protected:
    sf::Vector2f position;
    sf::Vector2f size;
    sf::RectangleShape m_collisionBox;  // 碰撞箱

private:

};