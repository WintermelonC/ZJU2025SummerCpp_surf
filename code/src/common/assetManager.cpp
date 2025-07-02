#include "common/assetManager.h"

std::map<Textures, sf::Texture> AssetManager::m_textures;
std::map<Fonts, sf::Font> AssetManager::m_fonts;

void AssetManager::loadAssets() {
    std::string path{"../../assets/"};
    std::string imagePath = path + "images/";
    std::string otherPath = imagePath + "other/";
    std::string playerPath = imagePath + "player/";
    std::string obstaclePath = imagePath + "obstacle/";
    std::string fontPath = path + "fonts/";

    // Textures
    loadTexture(Textures::water, otherPath + "water.png");
    loadTexture(Textures::scoreboard, otherPath + "scoreboard.png");
    loadTexture(Textures::button, otherPath + "button.png");
    loadTexture(Textures::start_icon, otherPath + "start_icon.png");
    // Player
    loadTexture(Textures::player_center_1, playerPath + "player_center_1.png");
    loadTexture(Textures::player_center_2, playerPath + "player_center_2.png");
    loadTexture(Textures::player_center_3, playerPath + "player_center_3.png");
    loadTexture(Textures::player_left_11, playerPath + "player_left_11.png");
    loadTexture(Textures::player_left_12, playerPath + "player_left_12.png");
    loadTexture(Textures::player_left_13, playerPath + "player_left_13.png");
    loadTexture(Textures::player_left_21, playerPath + "player_left_21.png");
    loadTexture(Textures::player_left_22, playerPath + "player_left_22.png");
    loadTexture(Textures::player_left_23, playerPath + "player_left_23.png");
    loadTexture(Textures::player_right_11, playerPath + "player_right_11.png");
    loadTexture(Textures::player_right_12, playerPath + "player_right_12.png");
    loadTexture(Textures::player_right_13, playerPath + "player_right_13.png");
    loadTexture(Textures::player_right_21, playerPath + "player_right_21.png");
    loadTexture(Textures::player_right_22, playerPath + "player_right_22.png");
    loadTexture(Textures::player_right_23, playerPath + "player_right_23.png");
    loadTexture(Textures::player_stop_1, playerPath + "player_stop_1.png");
    loadTexture(Textures::player_stop_2, playerPath + "player_stop_2.png");
    loadTexture(Textures::player_stop_3, playerPath + "player_stop_3.png");
    // PlayerState
    loadTexture(Textures::heart_1, playerPath + "heart_1.png");
    loadTexture(Textures::heart_2, playerPath + "heart_2.png");
    loadTexture(Textures::power_1, playerPath + "power_1.png");
    loadTexture(Textures::power_2, playerPath + "power_2.png");
    // Obstacle
    loadTexture(Textures::beach_1, obstaclePath + "beach_1.png");
    loadTexture(Textures::beach_2, obstaclePath + "beach_2.png");
    loadTexture(Textures::beach_3, obstaclePath + "beach_3.png");
    loadTexture(Textures::boat_1, obstaclePath + "boat_1.png");
    loadTexture(Textures::boat_2, obstaclePath + "boat_2.png");
    loadTexture(Textures::seaweed_11, obstaclePath + "seaweed_11.png");
    loadTexture(Textures::seaweed_12, obstaclePath + "seaweed_12.png");
    loadTexture(Textures::seaweed_13, obstaclePath + "seaweed_13.png");
    loadTexture(Textures::seaweed_21, obstaclePath + "seaweed_21.png");
    loadTexture(Textures::seaweed_22, obstaclePath + "seaweed_22.png");
    loadTexture(Textures::seaweed_23, obstaclePath + "seaweed_23.png");
    loadTexture(Textures::seaweed_31, obstaclePath + "seaweed_31.png");
    loadTexture(Textures::seaweed_32, obstaclePath + "seaweed_32.png");
    loadTexture(Textures::seaweed_33, obstaclePath + "seaweed_33.png");
    loadTexture(Textures::stone_1, obstaclePath + "stone_1.png");
    loadTexture(Textures::stone_2, obstaclePath + "stone_2.png");
    loadTexture(Textures::stone_3, obstaclePath + "stone_3.png");
    loadTexture(Textures::stone_4, obstaclePath + "stone_4.png");
    loadTexture(Textures::stone_5, obstaclePath + "stone_5.png");
    loadTexture(Textures::wood_1, obstaclePath + "wood_1.png");
    loadTexture(Textures::wood_2, obstaclePath + "wood_2.png");
    loadTexture(Textures::wood_3, obstaclePath + "wood_3.png");
    loadTexture(Textures::wood_4, obstaclePath + "wood_4.png");
    loadTexture(Textures::wood_5, obstaclePath + "wood_5.png");
    loadTexture(Textures::wood_6, obstaclePath + "wood_6.png");
    
    // Font
    loadFont(Fonts::MSYHBD, fontPath + "MSYHBD.TTC");
}

sf::Texture& AssetManager::getTexture(const Textures& texture) {
    auto it = m_textures.find(texture);
    if (it != m_textures.end()) {
        return it -> second;
    } else {
        throw std::runtime_error("Texture not found");
    }
}

sf::Font& AssetManager::getFont(const Fonts& font) {
    auto it = m_fonts.find(font);
    if (it != m_fonts.end()) {
        return it -> second;
    } else {
        throw std::runtime_error("Font not found");
    }
}

void AssetManager::loadTexture(const Textures& texture, const std::string& filePath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filePath)) {
        throw std::runtime_error("Failed to load texture from " + filePath);
    }
    m_textures[texture] = std::move(tex);
}

void AssetManager::loadFont(const Fonts& font, const std::string& filePath) {
    sf::Font f;
    if (!f.openFromFile(filePath)) {
        throw std::runtime_error("Failed to load font from " + filePath);
    }
    m_fonts[font] = std::move(f);
}
