#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../models/gameModel.h"
#include "utils.h"

// 回调函数类型定义
using FocusLostCallback = std::function<void()>;
using FocusGainedCallback = std::function<void()>;
using MouseRightClickCallback = std::function<void()>;
using MouseLeftClickCallback = std::function<void(const bool&, 
                                                  const bool&,
                                                  const bool&)>;
using KeyPressCallback = std::function<void(const sf::Event::KeyPressed&)>;

class GameView {
public:
    GameView();

    bool initialize(unsigned int width, unsigned int height, const std::string& title);
    void updateWindowSize(const sf::Vector2u& size);
    
    // 事件处理
    void handleEvents();

    void renderGameplay();
    void renderStartMenu();
    void renderPauseMenu();
    void renderGameOver();
    void renderBackground();
    void display();
    void reset();

    // 回调函数设置
    void setOnFocusLost(FocusLostCallback callback) { m_onFocusLost = callback; }
    void setOnFocusGained(FocusGainedCallback callback) { m_onFocusGained = callback; }
    void setOnMouseRightClick(MouseRightClickCallback callback) { m_onMouseRightClick = callback; }
    void setOnMouseLeftClick(MouseLeftClickCallback callback) { m_onMouseLeftClick = callback; }
    void setOnKeyPress(KeyPressCallback callback) { m_onKeyPress = callback; }

    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }
    void setStartButton(const std::unique_ptr<sf::Sprite>* startButton) { m_startButton = startButton; }
    void setStartIcon(const std::unique_ptr<sf::Sprite>* startIcon) { m_startIcon = startIcon; }
    void setContinueButton(const std::unique_ptr<sf::Sprite>* continueButton) { m_continueButton = continueButton; }
    void setContinueIcon(const std::unique_ptr<sf::Sprite>* continueIcon) { m_continueIcon = continueIcon; }
    void setReturnButton(const std::unique_ptr<sf::Sprite>* returnButton) { m_returnButton = returnButton; }
    void setFont(const std::unique_ptr<sf::Font>* font) { MSYHBD_font = font; }
    void setObstacleItemSprites(std::vector<sf::Sprite>& obstacleItemSprites) { m_obstacleItemSprites = &obstacleItemSprites; }

    sf::RenderWindow& getWindow() { return m_window; }
    const sf::Vector2f getMousePos() const { return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)); }
    
private:
    sf::Text renderText(
        const sf::Font& font,
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter = true
    );

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    const std::unique_ptr<sf::Sprite>* m_water;
    const std::unique_ptr<sf::Sprite>* m_player;
    const std::unique_ptr<sf::Sprite>* m_startButton;
    const std::unique_ptr<sf::Sprite>* m_startIcon;
    const std::unique_ptr<sf::Sprite>* m_continueButton;
    const std::unique_ptr<sf::Sprite>* m_continueIcon;
    const std::unique_ptr<sf::Sprite>* m_returnButton;
    const std::vector<sf::Sprite>* m_startMenuPlayerAnimation;
    
    const std::unique_ptr<sf::Font>* MSYHBD_font;

    std::vector<sf::Sprite>* m_obstacleItemSprites;
    
    // 回调函数
    FocusLostCallback m_onFocusLost;
    FocusGainedCallback m_onFocusGained;
    MouseRightClickCallback m_onMouseRightClick;
    MouseLeftClickCallback m_onMouseLeftClick;
    KeyPressCallback m_onKeyPress;
};