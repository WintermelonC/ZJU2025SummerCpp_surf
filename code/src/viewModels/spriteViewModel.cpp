#include "spriteViewModel.h"

bool SpriteViewModel::initialize() {
    m_textureViewModel = std::make_unique<TextureViewModel>();
    if (!m_textureViewModel->initialize()) {
        return false;
    }

    bool success = true;

    success &= loadSprite(SpriteType::water, TextureType::water);
    success &= loadSprite(SpriteType::player, TextureType::player_center_1);
    success &= loadSprite(SpriteType::start_button, TextureType::button);
    success &= loadSprite(SpriteType::start_icon, TextureType::start_icon);
    success &= loadSprite(SpriteType::continue_button, TextureType::button);
    success &= loadSprite(SpriteType::continue_icon, TextureType::start_icon);
    success &= loadSprite(SpriteType::return_button, TextureType::button);


    return success;
}

void SpriteViewModel::setSprite(
    const SpriteType& spriteType,
    const sf::Color& color,
    const sf::Vector2f& position,
    const sf::Vector2f& scale,
    const bool& ifCenter
) {
    auto sprite = m_sprites.find(spriteType)->second;
    if (sprite != m_sprites.end()) {
        sprite->setColor(color);
        sprite->setPosition(position);
        sprite->setScale(scale);
        if (ifCenter) {
            sprite->second->setOrigin(
                sprite->getLocalBounds().size / 2.0f
            );
        }
    } else {
        std::cerr << "Sprite type not found: " << static_cast<int>(spriteType) << std::endl;
    }
}

bool SpriteViewModel::loadSprite(const SpriteType& spriteType, const TextureType& textureType) {
    const std::unique_ptr<sf::Texture>& texture = m_textureViewModel->getTexture(textureType);
    if (!texture) {
        std::cerr << "Failed to load texture for sprite type: " << static_cast<int>(spriteType) << std::endl;
        return false;
    }

    auto sprite = std::make_unique<sf::Sprite>(*texture);
    m_sprites[spriteType] = std::move(sprite);

    return true;
}