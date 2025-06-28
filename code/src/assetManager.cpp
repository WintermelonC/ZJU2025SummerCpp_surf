#include "assetManager.h"

std::map<Textures, sf::Texture> AssetManager::m_textures;

void AssetManager::loadAssets() {
    loadTexture(Textures::water, "../../assets/images/water.png");
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
