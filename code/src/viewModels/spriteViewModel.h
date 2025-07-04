#pragma once

#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "textureViewModel.h"

enum class SpriteType {
    water,
    player,
    start_button,
    start_icon,
    continue_button,
    continue_icon,
    return_button,
    scoreboard,
};

class SpriteViewModel {
public:
    bool initialize();

    const std::unique_ptr<sf::Sprite>* getSprite(const SpriteType& type) const { return &m_sprites.at(type); }
    const std::unique_ptr<sf::Texture>* getTexture(const TextureType& type) const { return &m_textureViewModel->getTexture(type); }
    const sf::Vector2f getSpriteSize(const SpriteType& type) const { return m_sprites.at(type)->getGlobalBounds().size; }
    sf::Sprite getNewSprite(const TextureType& textureType) const { return sf::Sprite(*m_textureViewModel->getTexture(textureType)); }

    void setSprite(
        const SpriteType& spriteType,
        const sf::Color& color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true
    );
    void setSprite(
        sf::Sprite& sprite,
        const sf::Color color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true
    );
    void setSpriteTexture(const SpriteType& spriteType, const TextureType& textureType) { m_sprites[spriteType]->setTexture(*m_textureViewModel->getTexture(textureType)); }
    void setSpriteTexture(sf::Sprite& sprite, const TextureType& textureType) { sprite.setTexture(*m_textureViewModel->getTexture(textureType)); }
    void setSpritePosition(const SpriteType& spriteType, const sf::Vector2f& position) { m_sprites[spriteType]->setPosition(position); }

private:
    bool loadSprite(const SpriteType& spriteType, const TextureType& textureType);
    void initializeButtonIcons();

private:
    std::unique_ptr<TextureViewModel> m_textureViewModel;

    std::map<SpriteType, std::unique_ptr<sf::Sprite>> m_sprites;
};