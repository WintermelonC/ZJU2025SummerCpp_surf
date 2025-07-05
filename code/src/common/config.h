#pragma once

#include <functional>
#include <SFML/Graphics.hpp>

namespace Config {
    // 回调函数类型定义
    using FocusLostCallback = std::function<void()>;
    using FocusGainedCallback = std::function<void()>;
    using MouseRightClickCallback = std::function<void()>;
    using MouseLeftClickCallback = std::function<void(const bool&, 
                                                    const bool&,
                                                    const bool&)>;
    using KeyPressCallback = std::function<void(const sf::Event::KeyPressed&)>;
    using PlayerUpdateCallback = std::function<void(const float&, const sf::Vector2f&, const sf::Vector2u&)>;
    using AnimationUpdateCallback = std::function<void(const float&)>;
    using GameUpdateCallback = std::function<void(const sf::Vector2u&)>;
    using ObstacleItemUpdateCallback = std::function<void(const float&)>;
    using SpriteUpdateCallback = std::function<void(const sf::Vector2u&)>;

    enum class GameState {
        startMenu,
        playing,
        paused,
        gameOver
    };

    enum class PlayerState {
        left2, left1, center, right1, right2, stop
    };

    struct Trail {
        sf::RectangleShape trail;
        float lifetime;  // 剩余存活时间
    };

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
        constexpr sf::Color BUTTON_COLOR = sf::Color(195, 240, 247);
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
