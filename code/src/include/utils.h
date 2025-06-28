/**
 * @file utils.h
 * @brief 工具类头文件
 * 
 * 管理字体、纹理等资源
 * 提供一些通用的工具函数，如渲染文本、渲染精灵等
 */

#pragma once

#include <string>
#include <map>
#include <SFML/Graphics.hpp>

constexpr sf::Vector2i RENDER_SIZE = {2560, 1440};  // 渲染区域大小
constexpr sf::Vector2f RENDER_CENTER_POS = static_cast<sf::Vector2f>(RENDER_SIZE) / 2.f;  // 渲染中心坐标

enum class Fonts {
    MSJHBD,
    almmdfdk
};

enum class Textures {
    water,
    start_button,
    start_icon,
    scoreboard,
    player_center_1, player_center_2, player_center_3,
    player_left_11, player_left_12, player_left_13,
    player_left_21, player_left_22, player_left_23,
    player_right_11, player_right_12, player_right_13,
    player_right_21, player_right_22, player_right_23,
    obstacle_1, obstacle_2
    player_stop_1, player_stop_2, player_stop_3,
    heart_1, heart_2, power_1, power_2
};

class Utils {
public:
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
    static sf::Text renderText(
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
     * @warning 如果不想改变纹理颜色，将 color 设置为 sf::Color::White
     */
    static sf::Sprite renderSprite(
        const Textures& texture,
        const sf::Color color,
        const sf::Vector2f position,
        const sf::Vector2f scale = {1.0f, 1.0f},
        const bool ifSmooth = true,
        const bool ifCenter = true
    );

    static void mouseHoverButton(
        sf::Sprite& button, 
        sf::Sprite& buttonShadow, 
        const sf::RenderWindow& window,
        const sf::Vector2f offset = {0.f, 0.f},
        const sf::Color color = {255, 255, 255}
    );

    static bool ifMouseOnButton(
        const sf::RenderWindow& window,
        const sf::Vector2f& buttonPos,
        const float& buttonWidth, 
        const float& buttonHeight
    );

    static sf::Font& getFont(const Fonts font);  // 获取字体
    static sf::Texture& getTexture(const Textures texture);  // 获取纹理

private:
    static void loadFont(const Fonts font);  // 加载字体
    static void loadTexture(const Textures texture);  // 加载纹理

private:
    static std::map<Fonts, std::string> m_fontPaths;  // 字体路径列表
    static std::map<Textures, std::string> m_texturePaths;  // 纹理路径列表

    static std::map<Fonts, sf::Font> m_fonts;  // 字体缓存
    static std::map<Textures, sf::Texture> m_textures;  // 纹理缓存
};
