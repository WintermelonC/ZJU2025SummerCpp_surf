#pragma once

#include <variant>
#include <SFML/Graphics.hpp>

struct WindowCloseEvent {};

using Event = std::variant<
    WindowCloseEvent
>;
