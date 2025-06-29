#pragma once

#include <SFML/Graphics.hpp>
#include "entityManager.h"
#include "config.h"

class RenderSystem {
public:
    void render(sf::RenderWindow& window);

    void renderBackground(sf::RenderWindow& window);

    void renderStartMenu(sf::RenderWindow& window);

    void renderPauseMenu(sf::RenderWindow& window);

    sf::Text renderText(const sf::Font& font, const std::string& content, const int size, const sf::Color& color, const sf::Vector2f& position, const bool ifCenter = true);

    sf::Sprite renderSprite(sf::Texture& texture, const sf::Color color, const sf::Vector2f& position, const sf::Vector2f& scale, const bool ifSmooth = true, const bool ifCenter = true);

private:
    void mouseHoverButton(sf::Sprite& button, sf::Sprite& buttonShadow, const sf::RenderWindow& window, const sf::Vector2f offset, const sf::Color color);

private:

};
