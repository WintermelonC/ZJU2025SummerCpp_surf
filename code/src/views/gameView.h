#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <deque>
#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../common/utils.h"

class GameView {
public:
    GameView();

    // 初始化和主循环
    void run();
    bool initialize(unsigned int width, unsigned int height, const std::string& title);
    void updateWindowSize(const sf::Vector2u& size);
    
    // 事件处理
    void handleEvents();

    // 渲染方法
    void renderGameplay();
    void renderStartMenu();
    void renderPauseMenu();
    void renderGameOver();
    void renderBackground();
    void display();
    void reset();

    // 回调函数设置
    void setOnFocusLost(Config::FocusLostCallback callback) { m_onFocusLost = callback; }
    void setOnFocusGained(Config::FocusGainedCallback callback) { m_onFocusGained = callback; }
    void setOnMouseRightClick(Config::MouseRightClickCallback callback) { m_onMouseRightClick = callback; }
    void setOnMouseLeftClick(Config::MouseLeftClickCallback callback) { m_onMouseLeftClick = callback; }
    void setOnKeyPress(Config::KeyPressCallback callback) { m_onKeyPress = callback; }
    void setPlayerUpdateCallback(Config::PlayerUpdateCallback callback) { m_playerUpdateCallback = callback; }
    void setUpdateCallback(Config::GameUpdateCallback callback) { m_GameViewModelUpdateCallback = std::move(callback); }
    void setObstacleItemUpdateCallback(Config::ObstacleItemUpdateCallback callback) { m_obstacleItemUpdateCallback = std::move(callback); }
    void setSpriteUpdateCallback(Config::SpriteUpdateCallback callback) { m_spriteUpdateCallback = std::move(callback); }
    void setAnimationUpdateCallback(Config::AnimationUpdateCallback callback) { m_animationUpdateCallback = std::move(callback); }

    // 设置渲染对象
    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }
    void setStartButton(const std::unique_ptr<sf::Sprite>* startButton) { m_startButton = startButton; }
    void setStartIcon(const std::unique_ptr<sf::Sprite>* startIcon) { m_startIcon = startIcon; }
    void setContinueButton(const std::unique_ptr<sf::Sprite>* continueButton) { m_continueButton = continueButton; }
    void setContinueIcon(const std::unique_ptr<sf::Sprite>* continueIcon) { m_continueIcon = continueIcon; }
    void setReturnButton(const std::unique_ptr<sf::Sprite>* returnButton) { m_returnButton = returnButton; }
    void setNewGameButton(const std::unique_ptr<sf::Sprite>* newGameButton) { m_newGameButton = newGameButton; }
    void setNewGameIcon(const std::unique_ptr<sf::Sprite>* newGameIcon) { m_newGameIcon = newGameIcon; }
    void setFont(const std::unique_ptr<sf::Font>* font) { MSYHBD_font = font; }
    void setObstacleItemSprites(const std::vector<sf::Sprite>& obstacleItemSprites) { m_obstacleItemSprites = &obstacleItemSprites; }
    void setEntityBounds(const std::vector<sf::FloatRect>& entityBounds) { m_entityBounds = &entityBounds; }
    void setScoreboard(const std::unique_ptr<sf::Sprite>* scoreboard) { m_scoreboard = scoreboard; }
    void setScore(const float* score) { m_score = score; }
    void setHighScore(const float* highScore) { m_highScore = highScore; }
    void setGameState(const Config::GameState* gameState) { m_gameState = gameState; }
    void setRipples(const std::deque<Config::Trail>* ripples) { m_ripples = ripples; }
    void setTails(const std::deque<Config::Trail>* tails) { m_tails = tails; }
    void setPlayerStartMenu(const std::unique_ptr<sf::Sprite>* playerStartMenu) { m_playerStartMenu = playerStartMenu; }
    void setHeartSprites(const std::vector<sf::Sprite>* heartSprites) { m_heartSprites = heartSprites; }
    void setPowerSprites(const std::vector<sf::Sprite>* powerSprites) { m_powerSprites = powerSprites; }

    // Getter 方法
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
    void mouseHoverButton(
        sf::Sprite& button, 
        sf::Sprite& buttonShadow, 
        const sf::Vector2f& mousePos,
        const sf::Vector2f offset = {0.f, 3.f},
        const sf::Color color = {255, 255, 255}
    );

private:
    // 核心渲染组件
    sf::RenderWindow m_window;
    sf::View m_view;
    sf::Clock m_clock;

    // 精灵对象指针
    const std::unique_ptr<sf::Sprite>* m_water;
    const std::unique_ptr<sf::Sprite>* m_player;
    const std::unique_ptr<sf::Sprite>* m_startButton;
    const std::unique_ptr<sf::Sprite>* m_startIcon;
    const std::unique_ptr<sf::Sprite>* m_continueButton;
    const std::unique_ptr<sf::Sprite>* m_continueIcon;
    const std::unique_ptr<sf::Sprite>* m_returnButton;
    const std::unique_ptr<sf::Sprite>* m_newGameButton;
    const std::unique_ptr<sf::Sprite>* m_scoreboard;
    const std::unique_ptr<sf::Sprite>* m_newGameIcon;
    const std::unique_ptr<sf::Font>* MSYHBD_font;
    const std::vector<sf::Sprite>* m_obstacleItemSprites;
    const std::vector<sf::FloatRect>* m_entityBounds;  // 存储已生成的障碍物组边界
    const float* m_score;
    const float* m_highScore;
    const std::vector<sf::Sprite>* m_startMenuPlayerAnimation;
    const Config::GameState* m_gameState;
    const std::deque<Config::Trail>* m_ripples;
    const std::deque<Config::Trail>* m_tails;
    const std::unique_ptr<sf::Sprite>* m_playerStartMenu;
    const std::vector<sf::Sprite>* m_heartSprites;
    const std::vector<sf::Sprite>* m_powerSprites;
    
    // 回调函数
    Config::FocusLostCallback m_onFocusLost;
    Config::FocusGainedCallback m_onFocusGained;
    Config::MouseRightClickCallback m_onMouseRightClick;
    Config::MouseLeftClickCallback m_onMouseLeftClick;
    Config::KeyPressCallback m_onKeyPress;
    Config::GameUpdateCallback m_GameViewModelUpdateCallback;
    Config::PlayerUpdateCallback m_playerUpdateCallback;
    Config::ObstacleItemUpdateCallback m_obstacleItemUpdateCallback;
    Config::SpriteUpdateCallback m_spriteUpdateCallback;
    Config::AnimationUpdateCallback m_animationUpdateCallback;
};