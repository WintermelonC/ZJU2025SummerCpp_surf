#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

namespace Config {
    // 回调函数类型定义
    using FocusLostCallback = std::function<void()>;
    using FocusGainedCallback = std::function<void()>;
    using MouseRightClickCallback = std::function<void()>;
    using MouseLeftClickCallback = std::function<void(const bool&, const bool&, const bool&)>;
    using KeyPressCallback = std::function<void(const sf::Event::KeyPressed&)>;
    using PlayerUpdateCallback = std::function<void(const float&, const sf::Vector2f&, const sf::Vector2u&)>;
    using AnimationUpdateCallback = std::function<void(const float&)>;
    using GameUpdateCallback = std::function<void(const sf::Vector2u&)>;
    using ObstacleItemUpdateCallback = std::function<void(const float&)>;
    using SpriteUpdateCallback = std::function<void(const sf::Vector2u&)>;

    // 枚举类型定义
    enum class GameState {
        startMenu,
        playing,
        paused,
        gameOver
    };

    enum class PlayerState {
        left2, 
        left1, 
        center, 
        right1, 
        right2, 
        stop
    };

    // 结构体定义
    struct Trail {
        sf::RectangleShape trail;
        float lifetime;  // 剩余存活时间
    };

    // 窗口配置
    namespace Window {
        constexpr sf::Vector2i RENDER_SIZE{2560, 1440};
        constexpr sf::Vector2f RENDER_CENTER{RENDER_SIZE.x / 2.0f, RENDER_SIZE.y / 2.0f};
        constexpr sf::Vector2i WINDOW_SIZE{720, 1280};

        // 按钮配置
        constexpr sf::Vector2f START_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);
        constexpr sf::Vector2f START_BUTTON_SCALE = {1.2f, 1.2f};
        constexpr sf::Vector2f CONTINUE_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 200.f);
        constexpr sf::Vector2f CONTINUE_BUTTON_SCALE = {1.2f, 1.2f};
        constexpr sf::Vector2f RETURN_BUTTON_POS = RENDER_CENTER + sf::Vector2f(0.f, 300.f);
        constexpr sf::Vector2f RETURN_BUTTON_SCALE = {0.8f, 0.8f};
        constexpr sf::Vector2i BUTTON_SIZE = {180, 60};
    }

    // 纹理配置
    namespace Texture {
        constexpr sf::Vector2i WATER_SIZE{256, 256};
        constexpr sf::Color BUTTON_COLOR = sf::Color(195, 240, 247);
        constexpr sf::Vector2f SIZE_1{32, 32};
        constexpr sf::Vector2f SIZE_2{64, 64};
        constexpr sf::Vector2f SIZE_3{128, 64};
        constexpr sf::Vector2f SIZE_4{64, 128};
        constexpr sf::Vector2f SIZE_5{128, 128};
        constexpr sf::Vector2f SIZE_6{192, 256};
        constexpr sf::Vector2f SIZE_7{192, 128};
    }

    // 玩家配置
    namespace Player {
        constexpr sf::Vector2f PLAYER_POS{Config::Window::RENDER_CENTER.x, Config::Window::RENDER_SIZE.y / 5.0f * 2.0f};
        constexpr sf::Vector2f PLAYER_SIZE{64, 96};
        constexpr sf::Vector2f PLAYER_SCALE = {1.5f, 1.5f};
        constexpr int PLAYER_HP = 3;
        constexpr int PLAYER_POWER = 3;
        constexpr float POWER_DURATION = 5.0f;     // 能量持续时间（秒）
        constexpr float POWER_MULTIPLIER = 1.5f;   // 能量加速倍数
        constexpr float SPEED_THRESHOLD_1 = 20.0f; // 速度阈值常量
        constexpr float SPEED_THRESHOLD_2 = 40.0f;
    }

    // 游戏配置
    namespace Game {
        constexpr float PARALLAX_FACTOR = 0.1f;  // 视差因子
    }
}
