#pragma once

#include <memory>
#include <string>
#include <iostream>
#include <functional>
#include <SFML/Graphics.hpp>
#include "../common/surfMap.h"

class GameView {
public:
    GameView();

    bool initialize(unsigned int width, unsigned int height, const std::string& title);
    void run();

    void setMap(const SurfMap* map) { m_map = map; }
    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }
    void setUpdateCallback(std::function<void(const sf::Vector2f&)> callback) { m_updateCallback = std::move(callback); }

private:
    void handleEvents();
    void render();

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    const SurfMap* m_map;

    const std::unique_ptr<sf::Sprite>* m_water;
    const std::unique_ptr<sf::Sprite>* m_player;

    std::function<void(const sf::Vector2f&)> m_updateCallback;
};