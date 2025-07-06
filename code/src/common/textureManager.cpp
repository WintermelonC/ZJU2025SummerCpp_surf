#include "textureManager.h"

TextureManager::TextureManager() {
    initialize();
}

bool TextureManager::initialize() {
    std::string assetsPath = "../../assets/";
    std::string imagePath = assetsPath + "images/";
    std::string otherPath = imagePath + "other/";
    std::string playerPath = imagePath + "player/";
    std::string obstaclePath = imagePath + "obstacle/";
    bool success = true;

    success &= loadTexture(TextureType::water, otherPath + "water.png");
    success &= loadTexture(TextureType::button, otherPath + "button.png");
    success &= loadTexture(TextureType::start_icon, otherPath + "start_icon.png");
    success &= loadTexture(TextureType::scoreboard, otherPath + "scoreboard.png");

    success &= loadTexture(TextureType::player_center_1, playerPath + "player_center_1.png");
    success &= loadTexture(TextureType::player_center_2, playerPath + "player_center_2.png");
    success &= loadTexture(TextureType::player_center_3, playerPath + "player_center_3.png");
    success &= loadTexture(TextureType::player_left_11, playerPath + "player_left_11.png");
    success &= loadTexture(TextureType::player_left_12, playerPath + "player_left_12.png");
    success &= loadTexture(TextureType::player_left_13, playerPath + "player_left_13.png");
    success &= loadTexture(TextureType::player_left_21, playerPath + "player_left_21.png");
    success &= loadTexture(TextureType::player_left_22, playerPath + "player_left_22.png");
    success &= loadTexture(TextureType::player_left_23, playerPath + "player_left_23.png");
    success &= loadTexture(TextureType::player_right_11, playerPath + "player_right_11.png");
    success &= loadTexture(TextureType::player_right_12, playerPath + "player_right_12.png");
    success &= loadTexture(TextureType::player_right_13, playerPath + "player_right_13.png");
    success &= loadTexture(TextureType::player_right_21, playerPath + "player_right_21.png");
    success &= loadTexture(TextureType::player_right_22, playerPath + "player_right_22.png");
    success &= loadTexture(TextureType::player_right_23, playerPath + "player_right_23.png");
    success &= loadTexture(TextureType::player_stop_1, playerPath + "player_stop_1.png");
    success &= loadTexture(TextureType::player_stop_2, playerPath + "player_stop_2.png");
    success &= loadTexture(TextureType::player_stop_3, playerPath + "player_stop_3.png");
    success &= loadTexture(TextureType::heart_1, playerPath + "heart_1.png");
    success &= loadTexture(TextureType::heart_2, playerPath + "heart_2.png");
    success &= loadTexture(TextureType::power_1, playerPath + "power_1.png");
    success &= loadTexture(TextureType::power_2, playerPath + "power_2.png");
    success &= loadTexture(TextureType::wood_1, obstaclePath + "wood_1.png");
    success &= loadTexture(TextureType::wood_2, obstaclePath + "wood_2.png");
    success &= loadTexture(TextureType::wood_3, obstaclePath + "wood_3.png");
    success &= loadTexture(TextureType::wood_4, obstaclePath + "wood_4.png");
    success &= loadTexture(TextureType::wood_5, obstaclePath + "wood_5.png");
    success &= loadTexture(TextureType::wood_6, obstaclePath + "wood_6.png");
    success &= loadTexture(TextureType::stone_1, obstaclePath + "stone_1.png");
    success &= loadTexture(TextureType::stone_2, obstaclePath + "stone_2.png");
    success &= loadTexture(TextureType::stone_3, obstaclePath + "stone_3.png");
    success &= loadTexture(TextureType::stone_4, obstaclePath + "stone_4.png");
    success &= loadTexture(TextureType::stone_5, obstaclePath + "stone_5.png");
    success &= loadTexture(TextureType::boat_1, obstaclePath + "boat_1.png");
    success &= loadTexture(TextureType::boat_2, obstaclePath + "boat_2.png");
    success &= loadTexture(TextureType::beach_1, obstaclePath + "beach_1.png");
    success &= loadTexture(TextureType::beach_2, obstaclePath + "beach_2.png");
    success &= loadTexture(TextureType::beach_3, obstaclePath + "beach_3.png");
    success &= loadTexture(TextureType::seaweed_11, obstaclePath + "seaweed_11.png");
    success &= loadTexture(TextureType::seaweed_21, obstaclePath + "seaweed_21.png");
    success &= loadTexture(TextureType::seaweed_31, obstaclePath + "seaweed_31.png");
    success &= loadTexture(TextureType::bridge_1, obstaclePath + "bridge_1.png");

    return success;
}

bool TextureManager::loadTexture(const TextureType& type, const std::string& filePath) {
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filePath)) {
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return false;
    }
    m_textures[type] = std::move(texture);
    return true;
}