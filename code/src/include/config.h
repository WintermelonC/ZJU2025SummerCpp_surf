#pragma once

#include <SFML/Graphics.hpp>

namespace Config {
    namespace Window {
        constexpr sf::Vector2i RENDER_SIZE{2560, 1440};
        constexpr sf::Vector2f RENDER_CENTER{RENDER_SIZE.x / 2.0f, RENDER_SIZE.y / 2.0f};
        constexpr sf::Vector2i WINDOW_SIZE{720, 1280};
    }

    namespace Texture {
        constexpr sf::Vector2i WATER_SIZE{256, 256};
    }
}
