#pragma once

#include <iostream>
#include <string>
#include <memory>
#include <map>
#include <SFML/Graphics.hpp>

enum class TextureType {
    water,
    button,
    start_icon,
    scoreboard,
    player_center_1, player_center_2, player_center_3,
    player_left_11, player_left_12, player_left_13,
    player_left_21, player_left_22, player_left_23,
    player_right_11, player_right_12, player_right_13,
    player_right_21, player_right_22, player_right_23,
    player_stop_1, player_stop_2, player_stop_3,
    heart_1, heart_2, power_1, power_2,
    wood_1, wood_2, wood_3, wood_4, wood_5, wood_6,
    stone_1, stone_2, stone_3, stone_4, stone_5,
    boat_1, boat_2, 
    beach_1, beach_2, beach_3,
    seaweed_11, seaweed_21, seaweed_31,
    bridge_1,
};

class TextureViewModel {
public:
    bool initialize();

    const std::unique_ptr<sf::Texture>& getTexture(const TextureType& type) const { return m_textures.at(type); }

private:
    bool loadTexture(const TextureType& type, const std::string& filePath);

private:
    std::map<TextureType, std::unique_ptr<sf::Texture>> m_textures;
};