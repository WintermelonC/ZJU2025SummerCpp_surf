#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../common/config.h"

class GameView {
public:
    GameView();

    bool initialize(unsigned int width, unsigned int height, const std::string& title);
    void updateWindowSize(const sf::Vector2u& size);

    void renderGameplay();
    void renderStartMenu();
    void renderPauseMenu();
    void renderGameOver();
    void renderBackground();
    void display();
    void reset();

    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }
    void setStartButton(const std::unique_ptr<sf::Sprite>* startButton) { m_startButton = startButton; }
    void setStartIcon(const std::unique_ptr<sf::Sprite>* startIcon) { m_startIcon = startIcon; }
    void setContinueButton(const std::unique_ptr<sf::Sprite>* continueButton) { m_continueButton = continueButton; }
    void setReturnButton(const std::unique_ptr<sf::Sprite>* returnButton) { m_returnButton = returnButton; }
    void setFont(const std::unique_ptr<sf::Font>* font) { MSYHBD_font = font; }
    void setObstacleItemSprites(std::vector<sf::Sprite>& obstacleItemSprites) { m_obstacleItemSprites = &obstacleItemSprites; }
    void setScoreboard(const std::unique_ptr<sf::Sprite>* scoreboard) { m_scoreboard = scoreboard; }
    void setScore(const float* score) { m_score = score; }

    sf::RenderWindow& getWindow() { return m_window; }
    const sf::Vector2u getWindowSize() const { return m_window.getSize(); }
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
    const std::unique_ptr<sf::Sprite>* m_scoreboard;
    
    const std::unique_ptr<sf::Font>* MSYHBD_font;

    const std::vector<sf::Sprite>* m_obstacleItemSprites;
    const float* m_score;
};