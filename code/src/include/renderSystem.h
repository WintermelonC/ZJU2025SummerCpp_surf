#pragma once

#include <deque>
#include <SFML/Graphics.hpp>
#include "entityManager.h"
#include "config.h"
#include "utils.h"

class RenderSystem {
public:
    void renderPlayer(
        sf::RenderWindow& window, 
        const int& HP, 
        const int& power,
        const float& score,
        const sf::Angle& playerAngle
    );
    void renderStartMenu(sf::RenderWindow& window);
    void renderPauseMenu(sf::RenderWindow& window);
    void renderBackground(sf::RenderWindow& window);
    void renderRipple(sf::RenderWindow& window);
    void renderTail(sf::RenderWindow& window);
    void renderEntities(sf::RenderWindow& window);

    void updateRipple(const float& dt, const sf::Vector2f& velocity, const bool& ifSpawn = false);
    void updateTail(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn = false);

#ifdef DEBUG
    void renderVelocity(sf::RenderWindow& window, const sf::Vector2f& velocity);
#endif  // DEBUG

private:
    /**
     * @brief 渲染文本
     * 
     * @param font 字体对象
     * @param content 文本内容
     * @param size 字体大小
     * @param color 文本颜色
     * @param position 文本位置
     * @param ifCenter 是否将 Origin 设置为中心
     * @param ifCovert 是否转换为宽文本
     * @return 渲染后的文本对象
     * 
     * @note 此函数通过 sf::String 自动处理 UTF-8 编码，可正确显示所有语言。
     */
    sf::Text renderText(
        const Fonts& font,
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter = true
    );

    /**
     * @brief 渲染精灵
     * 
     * @param path 纹理路径
     * @param color 颜色
     * @param position 位置
     * @param scale 缩放比例
     * @param ifCenter 是否将 Origin 设置为中心
     * @param ifSmooth 是否平滑纹理
     * 
     * @note 如果不想改变纹理颜色，将 color 设置为 sf::Color::White
     */
    sf::Sprite renderSprite(
        const Textures& texture,
        const sf::Color color,
        const sf::Vector2f position,
        const sf::Vector2f scale = {1.0f, 1.0f},
        const bool ifSmooth = true,
        const bool ifCenter = true
    );

    void renderPlayerAnimation(sf::RenderWindow& window);

    void spawnRipple(const bool& ifSpawn = false);
    void spawnTail(const sf::Angle& angle, const bool& ifSpawn = false);

    void mouseHoverButton(
        sf::Sprite& button, 
        sf::Sprite& buttonShadow, 
        const sf::Vector2f& mousePos,
        const sf::Vector2f offset = {0.f, 3.f},
        const sf::Color color = {255, 255, 255}
    );

private:
    const int RIPPLE_COUNT = 4;  // 水波数量
    const float RIPPLE_LIFETIME = 0.8f;  // 水波生命周期（秒）
    const int RIPPLE_ALPHA = 200;  // 水波初始透明度
    const sf::Color RIPPLE_COLOR = sf::Color(255, 255, 255, RIPPLE_ALPHA);  // 水波颜色
    const int TAIL_COUNT = 1;  // 拖尾数量
    const float TAIL_LIFETIME = 1.0f;  // 拖尾生命周期（秒）
    const int TAIL_ALPHA = 150;  // 拖尾初始透明度
    const sf::Color TAIL_COLOR = sf::Color(141, 249, 196, TAIL_ALPHA);
    const int HEART_X_OFFSET = 260;  // 生命值图标 X 坐标偏移量
    const int POWER_X_OFFSET = 100;  // 能量值图标 X 坐标偏移量
    const float HP_SCALE = 1.5f;  // 生命值图标缩放比例
    const float POWER_SCALE = HP_SCALE;  // 能量值图标缩放比例
    const float HP_GAP = 40.0f;  // 生命值图标之间的间隔
    const float POWER_GAP = HP_GAP;  // 能量值图标之间的间隔
    const sf::Color BUTTON_COLOR = sf::Color(195, 240, 247);  // 按钮颜色

    struct Trail {
        sf::RectangleShape trail;
        float lifetime;  // 剩余存活时间
    };

    std::deque<Trail> m_ripples;  // 水波
    std::deque<Trail> m_tails;  // 加速拖尾

    sf::Clock m_animClock;
    int m_currentAnimFrame = 0;
};
