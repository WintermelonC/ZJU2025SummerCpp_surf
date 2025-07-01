#pragma once

#include <SFML/Graphics.hpp>
#include "models/gameModel.h"
#include "models/playerModel.h"

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
    
    // 事件处理
    virtual bool isOpen() const = 0;
    virtual sf::Vector2i getMousePosition() const = 0;
    virtual sf::Vector2f mapPixelToCoords(const sf::Vector2i& pixel) const = 0;
    virtual void close() = 0;
    
    // 更新显示数据
    virtual void updatePlayerData(const PlayerModel& player) = 0;
    virtual void updateGameData(const GameModel& game) = 0;
    virtual void updateWaterOffset(const sf::Vector2f& offset) = 0;
};

// SFML实现的View
class SFMLGameView : public IGameView {
public:
    SFMLGameView();
    
    // IGameView接口实现
    void renderStartMenu() override;
    void renderGameplay() override;
    void renderPauseMenu() override;
    void renderGameOver() override;
    void display() override;
    void clear() override;
    
    bool isOpen() const override;
    sf::Vector2i getMousePosition() const override;
    sf::Vector2f mapPixelToCoords(const sf::Vector2i& pixel) const override;
    void close() override;
    
    // 更新显示数据
    void updatePlayerData(const PlayerModel& player) override;
    void updateGameData(const GameModel& game) override;
    void updateWaterOffset(const sf::Vector2f& offset) override;
    
    // 获取窗口引用（用于事件处理）
    sf::RenderWindow& getWindow() { return m_window; }

private:
    void renderBackground();
    void renderPlayer();
    void renderUI();
    void renderWater();
    
private:
    sf::RenderWindow m_window;
    sf::View m_view;
    
    // 缓存的渲染数据
    PlayerModel m_playerData;
    GameModel m_gameData;
    sf::Vector2f m_waterOffset;
};
