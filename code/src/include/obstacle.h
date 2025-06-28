#pragma once

#include "entity.h" // 包含基类头文件

constexpr int OBSTACLE_NUM = 2; // 障碍物数量

class Obstacle : public Entity {
public:
    explicit Obstacle(sf::Sprite sprite, Textures type):
        Entity(std::move(sprite), type) {}

    explicit Obstacle(const Obstacle& other):
        Entity(other.m_sprite, other.m_type) {}

    ~Obstacle() = default;

    // 使用 override 关键字，明确这是一个重写操作
    void update(float dt, const sf::Vector2f& playerVelocity) override;

};