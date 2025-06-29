#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite& water = EntityManager::getSprite(EntityType::water);
    sf::Sprite& player = EntityManager::getSprite(EntityType::player);

    window.draw(water);  // 绘制水
    window.draw(player); // 绘制玩家
}

sf::Text RenderSystem::renderText(
        const Fonts& font,
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter
    ) {
    // 获取字体指针
    sf::Font& fontPtr = AssetManager::getFont(font);
    sf::Text text(fontPtr);
    // 使用 sf::String 从 UTF-8 直接转换，完美支持所有语言
    text.setString(sf::String::fromUtf8(content.begin(), content.end()));
    text.setCharacterSize(size);
    text.setFillColor(color);

    if (ifCenter) {
        text.setOrigin(text.getLocalBounds().size / 2.0f);
    }
    text.setPosition(position);

    return text;
}

#ifdef DEBUG
void RenderSystem::renderVelocity(sf::RenderWindow& window, const sf::Vector2f& velocity) {
    // 渲染速度文本
    sf::Text velocityText = renderText(
        Fonts::MSYHBD,
        "Velocity: (" + std::to_string(static_cast<int>(velocity.x)) + ", " + 
                        std::to_string(static_cast<int>(velocity.y)) + ")",
        20,
        sf::Color::Black,
        Config::Window::RENDER_CENTER
    );

    window.draw(velocityText);
}
#endif
