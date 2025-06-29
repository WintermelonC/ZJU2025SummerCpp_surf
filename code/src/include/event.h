#pragma once

#include <variant>
#include <SFML/Graphics.hpp>

struct WindowCloseEvent {};
struct WindowResizeEvent {
    sf::Vector2u size;
};
struct WindowFocusLostEvent {};
struct WindowFocusGainedEvent {};
struct MouseLeftClickEvent {
    sf::Vector2i mousePos;
    sf::Vector2f worldPos;
};
struct MouseRightClickEvent {};
struct SpacePressedEvent {};

using Event = std::variant<
    WindowCloseEvent,
    WindowResizeEvent,
    WindowFocusLostEvent,
    WindowFocusGainedEvent,
    MouseLeftClickEvent,
    MouseRightClickEvent,
    SpacePressedEvent
>;
