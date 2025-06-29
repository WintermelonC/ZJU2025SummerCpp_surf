#pragma once

#include <SFML/Graphics.hpp>

namespace Config {
    namespace Window {
        constexpr sf::Vector2i RENDER_SIZE{2560, 1440};
        constexpr sf::Vector2f RENDER_CENTER{RENDER_SIZE.x / 2.0f, RENDER_SIZE.y / 2.0f};
        constexpr sf::Vector2i WINDOW_SIZE{720, 1280};

        constexpr sf::Vector2i BUTTON_SIZE = {180, 60};  // 按钮大小
        constexpr sf::Vector2f START_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);  // 开始按钮坐标
        constexpr sf::Vector2f START_BUTTON_SCALE = {1.2f, 1.2f};  // 开始按钮缩放比例
        constexpr sf::Vector2f CONTINUE_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);  // 继续按钮坐标
        constexpr sf::Vector2f CONTINUE_BUTTON_SCALE = {1.2f, 1.2f};  // 继续按钮缩放比例
        constexpr sf::Vector2f RETURN_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 300.f);  // 返回按钮坐标
        constexpr sf::Vector2f RETURN_BUTTON_SCALE = {0.8f, 0.8f};  // 返回按钮缩放比例

    }
    namespace Color {
        constexpr sf::Color BUTTON_COLOR{195, 240, 247};
    }

    namespace Texture {
        constexpr sf::Vector2i WATER_SIZE{256, 256};
    }
}
