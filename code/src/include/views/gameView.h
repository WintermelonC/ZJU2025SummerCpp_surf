#pragma once

#include <SFML/Graphics.hpp>
#include "models/gameModel.h"
#include "models/playerModel.h"
#include "views/renderSystem.h"
#include "views/IGameView.h"

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
    void reset();
    
    bool isOpen() const override;
    sf::Vector2i getMousePosition() const override;
    sf::Vector2f mapPixelToCoords(const sf::Vector2i& pixel) const override;
    void close() override;
    
    // 更新显示数据
    void updatePlayerData(const PlayerModel& player) override;
    void updateGameData(const GameModel& game) override;
    void updateWaterOffset(const sf::Vector2f& offset) override;
    void updateEffects(float deltaTime, const sf::Vector2f& velocity, const sf::Angle& angle, bool shouldSpawnRipple, bool shouldSpawnTail) override;
    
    // 输入处理接口实现
    void processInput(class EventBus& eventBus) override;
    void setViewSize(const sf::Vector2f& size) override;
    
    // 获取窗口引用（用于事件处理）
    sf::RenderWindow& getWindow() { return m_window; }

private:
    void renderPlayer();
    void renderWater();
    
private:
    sf::RenderWindow m_window;
    sf::View m_view;
    
    // 渲染系统
    RenderSystem m_renderSystem;
    
    // 缓存的渲染数据
    PlayerModel m_playerData;
    GameModel m_gameData;
    sf::Vector2f m_waterOffset;
};
