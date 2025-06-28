#include "assetManager.h"

std::map<Textures, sf::Texture> AssetManager::m_textures;

void AssetManager::loadAssets() {
    loadTexture(Textures::water, "../../assets/images/water.png");
    // Player
    loadTexture(Textures::player_center_1, "../../assets/images/player/player_center_1.png");
    loadTexture(Textures::player_center_2, "../../assets/images/player/player_center_2.png");
    loadTexture(Textures::player_center_3, "../../assets/images/player/player_center_3.png");
    loadTexture(Textures::player_left_11, "../../assets/images/player/player_left_11.png");
    loadTexture(Textures::player_left_12, "../../assets/images/player/player_left_12.png");
    loadTexture(Textures::player_left_13, "../../assets/images/player/player_left_13.png");
    loadTexture(Textures::player_left_21, "../../assets/images/player/player_left_21.png");
    loadTexture(Textures::player_left_22, "../../assets/images/player/player_left_22.png");
    loadTexture(Textures::player_left_23, "../../assets/images/player/player_left_23.png");
    loadTexture(Textures::player_right_11, "../../assets/images/player/player_right_11.png");
    loadTexture(Textures::player_right_12, "../../assets/images/player/player_right_12.png");
    loadTexture(Textures::player_right_13, "../../assets/images/player/player_right_13.png");
    loadTexture(Textures::player_right_21, "../../assets/images/player/player_right_21.png");
    loadTexture(Textures::player_right_22, "../../assets/images/player/player_right_22.png");
    loadTexture(Textures::player_right_23, "../../assets/images/player/player_right_23.png");
    loadTexture(Textures::player_stop_1, "../../assets/images/player/player_stop_1.png");
    loadTexture(Textures::player_stop_2, "../../assets/images/player/player_stop_2.png");
    loadTexture(Textures::player_stop_3, "../../assets/images/player/player_stop_3.png");
}

sf::Texture& AssetManager::getTexture(const Textures& texture) {
    auto it = m_textures.find(texture);
    if (it != m_textures.end()) {
        return it -> second;
    } else {
        throw std::runtime_error("Texture not found");
    }
}

void AssetManager::loadTexture(const Textures& texture, const std::string& filePath) {
    sf::Texture tex;
    if (!tex.loadFromFile(filePath)) {
        throw std::runtime_error("Failed to load texture from " + filePath);
    }
    m_textures[texture] = std::move(tex);
}
