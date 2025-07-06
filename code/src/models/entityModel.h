#pragma once

#include <SFML/Graphics.hpp>

// 前向声明
enum class ObstacleType;
enum class ItemType;

enum class EntityModelType {
    player,
    obstacle,
    item
};

class EntityModel {
public:
    EntityModel(const EntityModelType& type) : m_type(type) {}

    EntityModel(const EntityModelType& type,
                const sf::Vector2f& pos,
                const sf::Vector2f& sz)
        : m_type(type), position(pos), size(sz) {}

    virtual ~EntityModel() = default;

    // Getter 方法
    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getSize() const { return size; }
    const sf::RectangleShape& getCollisionBox() const { return m_collisionBox; }
    const bool isObstacle() const { return m_type == EntityModelType::obstacle; }
    const bool isItem() const { return m_type == EntityModelType::item; }
    EntityModelType getType() const { return m_type; }

    // 虚函数用于获取具体类型
    virtual ObstacleType getObstacleType() const { return static_cast<ObstacleType>(0); }
    virtual ItemType getItemType() const { return static_cast<ItemType>(0); }

    // Setter 方法
    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setSize(const sf::Vector2f& sz) { size = sz; }
    void setCollisionBox(const sf::Vector2f& pos, 
                        const sf::Vector2f& origin, 
                        const sf::Vector2f& sz, 
                        const sf::Angle& angle, 
                        const sf::Color& color = sf::Color(255, 255, 255, 100)) {
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
    EntityModelType m_type;
};