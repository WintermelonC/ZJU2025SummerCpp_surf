#pragma once

#include <variant>
#include <SFML/Graphics.hpp>

struct WindowCloseEvent {};
struct WindowResizeEvent {
    sf::Vector2u size;
};
struct MouseRightClickEvent {};

using Event = std::variant<
    WindowCloseEvent,
    WindowResizeEvent,
    MouseRightClickEvent
>;
