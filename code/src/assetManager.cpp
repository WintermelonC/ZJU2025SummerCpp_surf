#include "assetManager.h"

std::map<Textures, sf::Texture> AssetManager::m_textures;
std::map<Fonts, sf::Font> AssetManager::m_fonts;

void AssetManager::loadAssets() {
    loadFont(Fonts::MSYHBD, "../../assets/fonts/MSYHBD.TTC");
    loadTexture(Textures::water, "../../assets/images/water.png");
    loadTexture(Textures::start_button, "../../assets/images/start_button.png");
    loadTexture(Textures::start_icon, "../../assets/images/start_icon.png");
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
    sf::Font fnt;
    if (!fnt.openFromFile(filePath)) {
        throw std::runtime_error("Failed to load font from " + filePath);
    }
    m_fonts[font] = std::move(fnt);
}
