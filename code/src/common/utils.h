#pragma once

#include <random>
#include <SFML/Graphics.hpp>

class Utils {
public:
    // 生成随机浮点数
    static float randomFloat(float a, float b) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_real_distribution<float> dist(a, b);
        return dist(rng);
    }

    // 生成随机整数
    static int randomInt(int a, int b) {
        static std::mt19937 rng(std::random_device{}());
        std::uniform_int_distribution<int> dist(a, b);
        return dist(rng);
    }

    // 检查鼠标是否在按钮上
    static bool ifMouseOnButton(
        const sf::Vector2f& mousePos,
        const sf::Vector2f& buttonPos,
        const sf::Vector2f& buttonSize
    ) {
        return (
            mousePos.x >= buttonPos.x - buttonSize.x / 2 && 
            mousePos.x <= buttonPos.x + buttonSize.x / 2 &&
            mousePos.y >= buttonPos.y - buttonSize.y / 2 && 
            mousePos.y <= buttonPos.y + buttonSize.y / 2
        );
    }

    // 设置精灵属性
    static void setSprite(
        sf::Sprite& sprite,
        const sf::Color color,
        const sf::Vector2f& position,
        const sf::Vector2f& scale = {1.0f, 1.0f},
        const bool& ifCenter = true
    ) {
        setSpriteColor(sprite, color);
        sprite.setPosition(position);
        sprite.setScale(scale);
        if (ifCenter) {
            sprite.setOrigin({
                sprite.getTexture().getSize().x / 2.f, 
                sprite.getTexture().getSize().y / 2.f
            });
        }
    }
    static void setSpriteColor(
        sf::Sprite& sprite,
        const sf::Color color
    ) {
        sprite.setColor(color);
    }

private:
    Utils() = default;  // 工具类不允许实例化
};