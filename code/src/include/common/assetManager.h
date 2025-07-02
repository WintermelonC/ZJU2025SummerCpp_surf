#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

enum class Fonts {
    MSYHBD
};

enum class Textures {
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
    beach_1, beach_2, beach_3,
    boat_1, boat_2,
    seaweed_11, seaweed_12, seaweed_13,
    seaweed_21, seaweed_22, seaweed_23,
    seaweed_31, seaweed_32, seaweed_33,
    stone_1, stone_2, stone_3, stone_4, stone_5,
    wood_1, wood_2, wood_3, wood_4, wood_5, wood_6
};

class AssetManager {
public:
    static void loadAssets();
    static sf::Texture& getTexture(const Textures& texture);
    static sf::Font& getFont(const Fonts& font);

private:
    static void loadTexture(const Textures& texture, const std::string& filePath);
    static void loadFont(const Fonts& font, const std::string& filePath);

private:
    static std::map<Textures, sf::Texture> m_textures;  // 存储纹理
    static std::map<Fonts, sf::Font> m_fonts;  // 存储字体
};
