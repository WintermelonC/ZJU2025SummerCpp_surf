#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

enum class Textures {
    water,
    player_center_1, player_center_2, player_center_3,
    player_left_11, player_left_12, player_left_13,
    player_left_21, player_left_22, player_left_23,
    player_right_11, player_right_12, player_right_13,
    player_right_21, player_right_22, player_right_23,
    player_stop_1, player_stop_2, player_stop_3
};

class AssetManager {
public:
    static void loadAssets();
    static sf::Texture& getTexture(const Textures& texture);

private:
    static void loadTexture(const Textures& texture, const std::string& filePath);

private:
    static std::map<Textures, sf::Texture> m_textures;  // 存储纹理
};
