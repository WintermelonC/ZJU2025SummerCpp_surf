#pragma once

#include <SFML/Graphics.hpp>

class EntityModel {
public:
    EntityModel(const sf::Vector2f& pos = {0, 0}, 
                const sf::Vector2f& sz = {1, 1})
        : position(pos), 
          size(sz) {};
    virtual ~EntityModel() = default;

    const sf::Vector2f& getPosition() const { return position; }
    const sf::Vector2f& getSize() const { return size; }

    void setPosition(const sf::Vector2f& pos) { position = pos; }
    void setSize(const sf::Vector2f& sz) { size = sz; }

protected:
    sf::Vector2f position;
    sf::Vector2f size;

private:

};