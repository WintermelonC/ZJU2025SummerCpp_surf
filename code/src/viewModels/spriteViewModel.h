#pragma once

#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../common/notificationCenter.h"
#include "../common/utils.h"
#include "../common/textureManager.h"

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

class SpriteViewModel : public INotificationObserver, public std::enable_shared_from_this<SpriteViewModel> {
public:
    SpriteViewModel(std::shared_ptr<TextureManager> textureVM);

    // 初始化和更新方法
    bool initialize();
    void update(const sf::Vector2u& windowSize);

    // 在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    // Getter 方法
    const std::unique_ptr<sf::Sprite>* getSprite(const SpriteType& type) const { 
        return &m_sprites.at(type); 
    }
    
    const std::unique_ptr<sf::Texture>* getTexture(const TextureType& type) const { 
        return &m_textureManager->getTexture(type); 
    }
    
    const sf::Vector2f getSpriteSize(const SpriteType& type) const { 
        return m_sprites.at(type)->getGlobalBounds().size; 
    }
    
    sf::Sprite getNewSprite(const TextureType& textureType) const { 
        return sf::Sprite(*m_textureManager->getTexture(textureType)); 
    }
    
    const std::unique_ptr<sf::Sprite>* getPlayerStartMenu() const { 
        return &m_playerStartMenu; 
    }

    // 获取回调方法
    Config::SpriteUpdateCallback getUpdateCommand();

    // Setter 方法
    void setSprite(
        const SpriteType& spriteType,
        const sf::Color& color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true
    );
    
    void setSpriteTexture(const SpriteType& spriteType, const TextureType& textureType) { 
        m_sprites[spriteType]->setTexture(*m_textureManager->getTexture(textureType)); 
    }
    
    void setSpritePosition(const SpriteType& spriteType, const sf::Vector2f& position) { 
        m_sprites[spriteType]->setPosition(position); 
    }
    
    void setPlayerPosition(const sf::Vector2f* playerPosition) { m_playerPosition = playerPosition; }
    void setPlayerTexture(const TextureType* playerTexture) { m_playerTexture = playerTexture; }
    void setGameState(const Config::GameState* gameState) { m_gameState = gameState; }
    void setWaterOffset(const sf::Vector2f* waterOffset) { m_waterOffset = waterOffset; }

private:
    // 初始化和更新方法
    bool loadSprite(const SpriteType& spriteType, const TextureType& textureType);
    void initializeButtonIcons();
    void updatePlayerStartMenu();
    void reset();

private:
    // 核心依赖
    std::shared_ptr<TextureManager> m_textureManager;

    // 精灵存储
    std::map<SpriteType, std::unique_ptr<sf::Sprite>> m_sprites;
    std::unique_ptr<sf::Sprite> m_playerStartMenu;
    
    // 动画相关
    sf::Clock m_animClock;
    int m_currentAnimFrame = 0;
    
    // 外部状态依赖
    const sf::Vector2f* m_playerPosition;
    const TextureType* m_playerTexture;
    const Config::GameState* m_gameState;
    const sf::Vector2f* m_waterOffset;
};