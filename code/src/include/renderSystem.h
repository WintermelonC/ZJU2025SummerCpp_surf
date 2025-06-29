#pragma once

#include <SFML/Graphics.hpp>
#include "entityManager.h"
#include "config.h"

class RenderSystem {
public:
    void render(sf::RenderWindow& window);
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
};
