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

private:
    std::string m_path{"../../assets/images/player/player_center_1.png"};
    sf::Texture m_texture;  // 玩家纹理
    sf::Sprite m_sprite;  // 玩家精灵
};
