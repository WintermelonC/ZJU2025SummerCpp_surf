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

    namespace Player {
        constexpr sf::Vector2f PLAYER_POS{ Config::Window::RENDER_CENTER.x, Config::Window::RENDER_SIZE.y / 5.0f * 2.0f};
        constexpr sf::Vector2i PLAYER_SIZE{64, 96};
        constexpr int PLAYER_HP = 3;  // 玩家最大生命值
        constexpr int PLAYER_POWER = 3;  // 玩家最大能量值
    }

    namespace Game {
        constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子
    }
}
