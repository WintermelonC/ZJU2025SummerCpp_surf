#pragma once

#include <variant>
#include <SFML/Graphics.hpp>
#include "entityManager.h"

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
struct ObstacleCollisionEvent {};
struct ItemCollisionEvent {
    EntityType itemType;  // 碰撞的物品类型
};

using Event = std::variant<
    WindowCloseEvent,
    WindowResizeEvent,
    WindowFocusLostEvent,
    WindowFocusGainedEvent,
    MouseLeftClickEvent,
    MouseRightClickEvent,
    SpacePressedEvent,
    ObstacleCollisionEvent,
    ItemCollisionEvent
>;
