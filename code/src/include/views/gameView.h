#pragma once

#include <SFML/Graphics.hpp>
#include "views/renderSystem.h"

// SFML实现的View
class SFMLGameView{
public:
    SFMLGameView();
    
    // 接口实现
    void renderStartMenu(const sf::Vector2f& waterOffset);
    void renderGameplay(
        float deltaTime,
        const sf::Vector2f& waterOffset, 
        const int & hp, 
        const int & power, 
        const float & score
    ) ;
    void renderPauseMenu(
        float deltaTime,
        const sf::Vector2f& waterOffset,
        const int & hp,
        const int & power,
        const float & score
    );
    void renderGameOver();
    void display();
    void clear();
    void reset();
    
    bool isOpen() const;
    sf::Vector2i getMousePosition() const;
    sf::Vector2f mapPixelToCoords(const sf::Vector2i& pixel) const;
    void close();
    
    // 更新显示数据
    void updateEffects(float deltaTime, const sf::Vector2f& velocity, const sf::Angle& angle, bool shouldSpawnRipple, bool shouldSpawnTail);
    
    // 输入处理接口实现
    void processInput(class EventBus& eventBus);
    void setViewSize(const sf::Vector2f& size);
    
    // 获取窗口引用（用于事件处理）
    sf::RenderWindow& getWindow() { return m_window; }

private:
    void renderPlayer(const int & hp, const int & power, const float & score);
    void renderWater(const sf::Vector2f& waterOffset);
    
private:
    sf::RenderWindow m_window;
    sf::View m_view;
    
    // 渲染系统
    RenderSystem m_renderSystem;
};
