#include "spriteViewModel.h"

bool SpriteViewModel::initialize() {
    m_textureViewModel = std::make_unique<TextureViewModel>();
    if (!m_textureViewModel->initialize()) {
        return false;
    }

    bool success = true;

    success &= loadSprite(SpriteType::water, TextureType::water);
    success &= loadSprite(SpriteType::player, TextureType::player_center_1);

    return success;
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