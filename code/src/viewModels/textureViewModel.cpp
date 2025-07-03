#include "textureViewModel.h"

bool TextureViewModel::initialize() {
    std::string assetsPath = "../../assets/";
    std::string imagePath = assetsPath + "images/";
    std::string otherPath = imagePath + "other/";
    std::string playerPath = imagePath + "player/";
    std::string obstaclePath = imagePath + "obstacle/";
    std::string fontPath = assetsPath + "fonts/";
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

    return success;
}

bool TextureViewModel::loadTexture(const TextureType& type, const std::string& filePath) {
    auto texture = std::make_unique<sf::Texture>();
    if (!texture->loadFromFile(filePath)) {
        std::cerr << "Failed to load texture from: " << filePath << std::endl;
        return false;
    }
    m_textures[type] = std::move(texture);
    return true;
}