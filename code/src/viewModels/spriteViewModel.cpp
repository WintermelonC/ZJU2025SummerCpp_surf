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
    success &= loadSprite(SpriteType::wood_1, TextureType::wood_1);
    success &= loadSprite(SpriteType::wood_2, TextureType::wood_2);
    success &= loadSprite(SpriteType::wood_3, TextureType::wood_3);
    success &= loadSprite(SpriteType::wood_4, TextureType::wood_4);
    success &= loadSprite(SpriteType::wood_5, TextureType::wood_5);
    success &= loadSprite(SpriteType::wood_6, TextureType::wood_6);
    success &= loadSprite(SpriteType::stone_1, TextureType::stone_1);
    success &= loadSprite(SpriteType::stone_2, TextureType::stone_2);
    success &= loadSprite(SpriteType::stone_3, TextureType::stone_3);
    success &= loadSprite(SpriteType::stone_4, TextureType::stone_4);
    success &= loadSprite(SpriteType::stone_5, TextureType::stone_5);
    success &= loadSprite(SpriteType::boat_1, TextureType::boat_1);
    success &= loadSprite(SpriteType::boat_2, TextureType::boat_2);
    success &= loadSprite(SpriteType::beach_1, TextureType::beach_1);
    success &= loadSprite(SpriteType::beach_2, TextureType::beach_2);
    success &= loadSprite(SpriteType::beach_3, TextureType::beach_3);

    return success;
}

void SpriteViewModel::setSprite(
    const SpriteType& spriteType,
    const sf::Color& color,
    const sf::Vector2f& position,
    const sf::Vector2f& scale,
    const bool& ifCenter
) {
    auto it = m_sprites.find(spriteType);
    if (it != m_sprites.end()) {
        sf::Sprite& sprite = *it->second;
        sprite.setColor(color);
        if (ifCenter) {
            sprite.setOrigin(sprite.getLocalBounds().size / 2.f);
        }
        sprite.setPosition(position);
        sprite.setScale(scale);
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

void SpriteViewModel::setSprite(
    sf::Sprite& sprite,
    const sf::Color color,
    const sf::Vector2f& position,
    const sf::Vector2f& scale,
    const bool& ifCenter
) {
    sprite.setColor(color);
    sprite.setPosition(position);
    sprite.setScale(scale);
    if( ifCenter) {
        sprite.setOrigin({sprite.getTexture().getSize().x / 2.f, sprite.getTexture().getSize().y / 2.f});
    }
}