#pragma once

#include <SFML/Graphics.hpp>

// 前置声明
class GameModel;
class PlayerModel;
class EventBus;

// View接口
class IGameView {
public:
    virtual ~IGameView() = default;
    
    // 渲染方法
    virtual void renderStartMenu() = 0;
    virtual void renderGameplay() = 0;
    virtual void renderPauseMenu() = 0;
    virtual void renderGameOver() = 0;
    virtual void display() = 0;
    virtual void clear() = 0;
    virtual void reset() = 0;
    
    // 事件处理
    virtual bool isOpen() const = 0;
    virtual sf::Vector2i getMousePosition() const = 0;
    virtual sf::Vector2f mapPixelToCoords(const sf::Vector2i& pixel) const = 0;
    virtual void close() = 0;
    
    // 输入处理接口
    virtual void processInput(EventBus& eventBus) = 0;
    virtual void setViewSize(const sf::Vector2f& size) = 0;
    
    // 更新显示数据
    virtual void updatePlayerData(const PlayerModel& player) = 0;
    virtual void updateGameData(const GameModel& game) = 0;
    virtual void updateWaterOffset(const sf::Vector2f& offset) = 0;
    virtual void updateEffects(float deltaTime, const sf::Vector2f& velocity, const sf::Angle& angle, bool shouldSpawnRipple, bool shouldSpawnTail) = 0;
};
