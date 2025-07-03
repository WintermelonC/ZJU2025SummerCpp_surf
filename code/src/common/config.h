#pragma once

#include <SFML/Graphics.hpp>

namespace Config {
    namespace Window {
        constexpr sf::Vector2i RENDER_SIZE{2560, 1440};
        constexpr sf::Vector2f RENDER_CENTER{RENDER_SIZE.x / 2.0f, RENDER_SIZE.y / 2.0f};
        constexpr sf::Vector2i WINDOW_SIZE{720, 1280};

        constexpr sf::Vector2f START_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);  // 开始按钮坐标
        constexpr sf::Vector2f START_BUTTON_SCALE = {1.2f, 1.2f};  // 开始按钮缩放比例
        constexpr sf::Vector2f CONTINUE_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);  // 继续按钮坐标
        constexpr sf::Vector2f CONTINUE_BUTTON_SCALE = {1.2f, 1.2f};  // 继续按钮缩放比例
        constexpr sf::Vector2f RETURN_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 300.f);  // 返回按钮坐标
        constexpr sf::Vector2f RETURN_BUTTON_SCALE = {0.8f, 0.8f};  // 返回按钮缩放比例
        constexpr sf::Vector2i BUTTON_SIZE = {180, 60};
    }

    namespace Texture {
        constexpr sf::Vector2i WATER_SIZE{256, 256};
    }

    namespace Player {
        constexpr sf::Vector2f PLAYER_POS{Config::Window::RENDER_CENTER.x, Config::Window::RENDER_SIZE.y / 5.0f * 2.0f};
        constexpr sf::Vector2f PLAYER_SIZE{64, 96};
        constexpr sf::Vector2f PLAYER_SCALE = {1.5f, 1.5f};  // 玩家缩放比例
        constexpr int PLAYER_HP = 3;  // 玩家最大生命值
        constexpr int PLAYER_POWER = 3;  // 玩家最大能量值
        constexpr float POWER_DURATION = 5.0f;  // 能量持续时间（秒）
        constexpr float POWER_MULTIPLIER = 1.5f;  // 能量加速倍数
        constexpr float SPEED_THRESHOLD_1 = 20.0f; // 速度阈值常量
        constexpr float SPEED_THRESHOLD_2 = 40.0f;
    
    }

    namespace Game {
        constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子
    }
}
