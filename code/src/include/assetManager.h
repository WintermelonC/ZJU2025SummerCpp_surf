#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

enum class Textures {
    water
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
