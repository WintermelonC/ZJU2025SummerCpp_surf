#pragma once

#include <SFML/Graphics.hpp>
#include "textureViewModel.h"

enum class SpriteType {
    water,
    player,
    start_button,
    start_icon,
    continue_button,
    continue_icon,
    return_button,
    
};

class SpriteViewModel {
public:
    bool initialize();

    const std::unique_ptr<sf::Sprite>* getSprite(const SpriteType& type) const { return &m_sprites.at(type); }
    const std::unique_ptr<sf::Texture>* getTexture(const TextureType& type) const { return &m_textureViewModel->getTexture(type); }
    const sf::Vector2f getSpriteSize(const SpriteType& type) const { return m_sprites.at(type)->getGlobalBounds().size; }

private:
    bool loadSprite(const SpriteType& spriteType, const TextureType& textureType);

private:
    std::unique_ptr<TextureViewModel> m_textureViewModel;

    std::map<SpriteType, std::unique_ptr<sf::Sprite>> m_sprites;
};