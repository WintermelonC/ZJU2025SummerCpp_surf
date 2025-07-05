#include "spriteViewModel.h"

SpriteViewModel::SpriteViewModel(std::shared_ptr<TextureViewModel> textureViewModel)
    : m_textureViewModel(std::move(textureViewModel)) {
    initialize();
}

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
    success &= loadSprite(SpriteType::scoreboard, TextureType::scoreboard);

    setSprite(
        SpriteType::water,
        sf::Color::White,
        {0, 0},
        {1, 1},
        false
    );
    setSprite(
        SpriteType::player,
        sf::Color::White,
        Config::Player::PLAYER_POS,
        Config::Player::PLAYER_SCALE
    );
    m_playerStartMenu = std::make_unique<sf::Sprite>(*m_textureViewModel->getTexture(TextureType::player_center_1));

    initializeButtonIcons();

    return success;
}

void SpriteViewModel::update(const sf::Vector2u& windowSize) {
    if (!m_gameState) {
        return;
    }
    if (*m_gameState == Config::GameState::startMenu) {
        updatePlayerStartMenu();
        return;
    }
    if (*m_gameState == Config::GameState::playing) {
        setSpriteTexture(SpriteType::player, *m_playerTexture);
        setSprite(
            SpriteType::scoreboard,
            Config::Texture::BUTTON_COLOR,
            {Config::Window::RENDER_CENTER.x,
            Config::Window::RENDER_CENTER.y - windowSize.y / 2 + 50}
        );
        setSpritePosition(SpriteType::water, *m_waterOffset);
    }
}

void SpriteViewModel::subscribeToNotifications() {
    // 订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
}

void SpriteViewModel::updatePlayerStartMenu() {
    const std::vector<TextureType> animations = {
        TextureType::player_left_21, TextureType::player_left_22, TextureType::player_left_23,
        TextureType::player_left_11, TextureType::player_left_12, TextureType::player_left_13,
        TextureType::player_center_1, TextureType::player_center_2, TextureType::player_center_3,
        TextureType::player_right_11, TextureType::player_right_12, TextureType::player_right_13,
        TextureType::player_right_21, TextureType::player_right_22, TextureType::player_right_23,
        TextureType::player_right_21, TextureType::player_right_22, TextureType::player_right_23,
        TextureType::player_right_11, TextureType::player_right_12, TextureType::player_right_13,
        TextureType::player_left_11, TextureType::player_left_12, TextureType::player_left_13,
        TextureType::player_left_21, TextureType::player_left_22, TextureType::player_left_23
    };

    const int count = animations.size();
    const float animInterval = 0.08f;  // 动画间隔时间

    // 每隔 animInterval 秒切换帧
    if (m_animClock.getElapsedTime().asSeconds() >= animInterval) {
        m_currentAnimFrame = (m_currentAnimFrame + 1) % count;  // 循环动画帧
        m_animClock.restart();  // 重置动画时钟
    }

    m_textureViewModel->setSpriteTexture(
        *m_playerStartMenu,
        animations[m_currentAnimFrame]
    );

    Utils::setSprite(
        *m_playerStartMenu,
        sf::Color::White,
        Config::Window::RENDER_CENTER,
        {2.0f, 2.0f}
    );
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

void SpriteViewModel::initializeButtonIcons() {
    setSprite(SpriteType::start_button, Config::Texture::BUTTON_COLOR, Config::Window::START_BUTTON_POS, Config::Window::START_BUTTON_SCALE);
    setSprite(SpriteType::start_icon, sf::Color::White, Config::Window::START_BUTTON_POS - sf::Vector2f{75.f, 0.f}, {0.9f, 0.9f});
    setSprite(SpriteType::continue_button, Config::Texture::BUTTON_COLOR, Config::Window::CONTINUE_BUTTON_POS, Config::Window::CONTINUE_BUTTON_SCALE);
    setSprite(SpriteType::continue_icon, sf::Color::White, Config::Window::CONTINUE_BUTTON_POS - sf::Vector2f{75.f, 0.f}, {0.9f, 0.9f});
    setSprite(SpriteType::return_button, Config::Texture::BUTTON_COLOR, Config::Window::RETURN_BUTTON_POS, Config::Window::RETURN_BUTTON_SCALE);
}

void SpriteViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            if (resetData.resetPlayerState) {
                reset();
            }
            break;
        }
        default:
            break;
    }
}

void SpriteViewModel::reset() {
    m_textureViewModel->setSpriteTexture(*m_playerStartMenu, TextureType::player_center_1);
    setSpritePosition(SpriteType::water, *m_waterOffset);
}