#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#incldue "common/config.h"

class GameView {
public:
    GameView();

    bool initialize(unsigned int width, unsigned int height, const std::string& title);
    void run();

    void handleEvents();
    void renderGameplay();
    void renderStartMenu();
    void renderPauseMenu();
    void renderGameOver();
    void renderBackground();
    void display();
    void reset();

    void setMap(const SurfMap* map) { m_map = map; }
    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }
    void setStartButton(const std::unique_ptr<sf::Sprite>* startButton) { m_startButton = startButton; }
    void setStartIcon(const std::unique_ptr<sf::Sprite>* startIcon) { m_startIcon = startIcon; }
    void setContinueButton(const std::unique_ptr<sf::Sprite>* continueButton) { m_continueButton = continueButton; }
    void setReturnButton(const std::unique_ptr<sf::Sprite>* returnButton) { m_returnButton = returnButton; }
    void setFont(const std::unique_ptr<sf::Font>* font) { m_font = font; }
    void setUpdateCallback(std::function<void(const sf::Vector2f&)> callback) { m_updateCallback = std::move(callback); }

private:
    sf::Text renderText(
        const sf::Font& font,
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter = true
    )

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
    
    const std::unique_ptr<sf::Font>* MSYHBD_font;


    std::function<void(const sf::Vector2f&)> m_updateCallback;
};