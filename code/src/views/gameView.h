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
    void handleEvents();
    void render();

    void setMap(const SurfMap* map) { m_map = map; }
    void setWater(const std::unique_ptr<sf::Sprite>* water) { m_water = water; }
    void setPlayer(const std::unique_ptr<sf::Sprite>* player) { m_player = player; }

    const sf::RenderWindow& getWindow() const { return m_window; }
    const sf::Vector2f getMousePos() const { return m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window)); }

private:
    sf::RenderWindow m_window;
    sf::View m_view;

    const SurfMap* m_map;

    const std::unique_ptr<sf::Sprite>* m_water;
    const std::unique_ptr<sf::Sprite>* m_player;
};