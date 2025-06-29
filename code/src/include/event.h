#pragma once

#include <variant>
#include <SFML/Graphics.hpp>

struct WindowCloseEvent {};
struct WindowResizeEvent {
    sf::Vector2u size;
};
struct MouseRightClickEvent {};
struct WindowFocusLostEvent {};
struct WindowFocusGainedEvent {};
struct WindowMouseLeftClickEvent {
    sf::Vector2i mousePos;
    sf::Vector2f worldPos;
};
struct WindowMouseRightClickEvent {};
struct SpacePressedEvent {};

using Event = std::variant<
    WindowCloseEvent,
    WindowResizeEvent,
    MouseRightClickEvent
    WindowFocusLostEvent,
    WindowFocusGainedEvent,
    WindowMouseLeftClickEvent,
    WindowMouseRightClickEvent,
    SpacePressedEvent
>;
