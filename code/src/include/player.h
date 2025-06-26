#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "utils.h"

class Player {
public:
    Player(sf::Vector2f position = {WINDOW_CENTER_X, WINDOW_CENTER_Y});
    ~Player() = default;

    const sf::Vector2f getPosition() const { return m_sprite.getPosition(); }
    const sf::Sprite& getSprite() const { return m_sprite; }
    const sf::Vector2f& getVelocity() const { return m_velocity; }

    // 更新玩家速度
    void update(float dt);

private:
    std::string m_path{"../../assets/images/player/player_center_1.png"};
    sf::Texture m_texture;  // 玩家纹理
    sf::Sprite m_sprite;  // 玩家精灵
    sf::Vector2f m_velocity;  // 玩家速度

    float m_acceleration = 5.0f;
    float m_maxSpeed = 25.0f;
};
