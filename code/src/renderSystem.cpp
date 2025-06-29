#include "renderSystem.h"

void RenderSystem::renderBackground(sf::RenderWindow& window) {
    // 渲染背景逻辑
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite water = m_entityManager.getSprite(EntityType::water, Textures::water);

    window.draw(water);  // 绘制水的精灵
}

void RenderSystem::render(sf::RenderWindow& window) {
    // 渲染背景
    renderBackground(window);

    // 显示渲染结果
    window.display();
}

sf::Text RenderSystem::renderText(const sf::Font& font, const std::string& content, const int size, const sf::Color& color, const sf::Vector2f& position, const bool ifCenter) {
    sf::Text text(font);
    text.setString(sf::String::fromUtf8(content.begin(), content.end()));  // 设置文本内容
    text.setCharacterSize(size);
    text.setFillColor(color);

    if (ifCenter) {
        text.setOrigin(text.getLocalBounds().size / 2.0f);
    }
    text.setPosition(position);
    return text;
}

sf::Sprite RenderSystem::renderSprite(sf::Texture& texture, const sf::Color color, const sf::Vector2f& position, const sf::Vector2f& scale, const bool ifSmooth, const bool ifCenter) {
    texture.setSmooth(ifSmooth);
    sf::Sprite sprite(texture);
    sprite.setColor(color);
    sprite.setPosition(position);
    sprite.setScale(scale);

    if (ifCenter) {
        sprite.setOrigin({texture.getSize().x / 2.f, texture.getSize().y / 2.f});
    }

    return sprite;
}

void RenderSystem::renderStartMenu(sf::RenderWindow& window) {
    // 渲染背景
    renderBackground(window);

    // 渲染开始菜单逻辑
    // 标题
    sf::Font& titleFont = AssetManager::getFont(Fonts::MSYHBD);
    sf::Text title = renderText(
        titleFont,
        "LET'S SURF",
        50,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) - sf::Vector2f{0.f, 200.f}
    );

    // 开始按钮
    sf::Texture & startButtonTexture = AssetManager::getTexture(Textures::start_button);
    sf::Sprite startButton = renderSprite(
        startButtonTexture,
        Config::Color::BUTTON_COLOR,
        Config::Window::START_BUTTON_POS,
        Config::Window::START_BUTTON_SCALE
    );

    // 开始按钮阴影
    sf::Sprite startButtonShadow = startButton;
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 开始icon
    sf::Sprite startIcon = renderSprite(
        AssetManager::getTexture(Textures::start_icon),
        sf::Color::White,
        Config::Window::START_BUTTON_POS - sf::Vector2f{75.f, 0.f},
        {0.9f, 0.9f}
    );

    // 开始游戏文字
    sf::Text startText = renderText(
        titleFont,
        "开始游戏",
        35,
        sf::Color::Black,
        Config::Window::START_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );

    // 鼠标悬停按钮变化
    mouseHoverButton(startButton, startButtonShadow, window, {0.f, 0.3f}, {255, 255, 255});

    window.draw(title);
    window.draw(startButtonShadow);
    window.draw(startButton);
    window.draw(startIcon);
    window.draw(startText);

    // 显示渲染结果
    window.display();
}

void RenderSystem::renderPauseMenu(sf::RenderWindow& window) {
    // 渲染背景
    renderBackground(window);

    // 半透明遮罩
    sf::RectangleShape filter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE));
    filter.setFillColor(sf::Color(255, 255, 255, 30));

    // 暂停文字
    sf::Font& titleFont = AssetManager::getFont(Fonts::MSYHBD);
    sf::Text pausedText = renderText(
        titleFont,
        "已暂停",
        75,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) + sf::Vector2f{0.f, -400.f},
        true
    );

    // 继续按钮
    sf::Texture & continueButtonTexture = AssetManager::getTexture(Textures::start_button);
    sf::Sprite continueButton = renderSprite(
        continueButtonTexture,
        Config::Color::BUTTON_COLOR,
        Config::Window::CONTINUE_BUTTON_POS,
        Config::Window::CONTINUE_BUTTON_SCALE
    );

    // 继续按钮阴影
    sf::Sprite continueButtonShadow = continueButton;
    continueButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    continueButtonShadow.move({0.f, 4.f});  // 向下偏移

    // icon
    sf::Sprite continueIcon = renderSprite(
        AssetManager::getTexture(Textures::start_icon),
        sf::Color::White,
        Config::Window::CONTINUE_BUTTON_POS - sf::Vector2f{75.f, 0.f},
        {0.9f, 0.9f}
    );

    // 继续游戏文字
    sf::Text continueText = renderText(
        titleFont,
        "继续游戏",
        35,
        sf::Color::Black,
        Config::Window::CONTINUE_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );

    // 返回按钮
    sf::Sprite returnButton = renderSprite(
        AssetManager::getTexture(Textures::start_button),
        Config::Color::BUTTON_COLOR,
        Config::Window::RETURN_BUTTON_POS,
        Config::Window::RETURN_BUTTON_SCALE
    );

    // 返回按钮阴影
    sf::Sprite returnButtonShadow = returnButton;
    returnButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    returnButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 返回文字
    sf::Text returnText = renderText(
        titleFont,
        "返回菜单",
        26,
        sf::Color::Black,
        Config::Window::RETURN_BUTTON_POS - sf::Vector2f{0.f, 5.f},
        true
    );

    // 鼠标悬停按钮变化
    mouseHoverButton(continueButton, continueButtonShadow, window, {0.f, 0.3f}, {255, 255, 255});
    mouseHoverButton(returnButton, returnButtonShadow, window, {0.f, 0.3f}, {255, 255, 255});

    // 绘制所有元素
    // window.draw(player);
    window.draw(filter);
    window.draw(pausedText);  // 绘制暂停文字
    window.draw(continueButtonShadow);  // 绘制继续按钮阴影
    window.draw(continueButton);  // 绘制继续按钮
    window.draw(continueIcon);  // 绘制继续图标
    window.draw(continueText);  // 绘制继续游戏文字
    window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    window.draw(returnButton);  // 绘制返回按钮
    window.draw(returnText);  // 绘制返回菜单文字

    // 显示渲染结果
    window.display();
}

void RenderSystem::mouseHoverButton(sf::Sprite& button, sf::Sprite& buttonShadow, const sf::RenderWindow& window, const sf::Vector2f offset, const sf::Color color) {
    sf::Vector2f buttonPos = button.getPosition();
    float buttonWidth = button.getGlobalBounds().size.x * button.getScale().x;
    float buttonHeight = button.getGlobalBounds().size.y * button.getScale().y;
    
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    if(
        worldPos.x >= buttonPos.x - buttonWidth / 2 && 
        worldPos.x <= buttonPos.x + buttonWidth / 2 &&
        worldPos.y >= buttonPos.y - buttonHeight / 2 && 
        worldPos.y <= buttonPos.y + buttonHeight / 2
    )
    {
        button.setColor(sf::Color(255, 255, 255));  // 改变颜色
        buttonShadow.move({0.f, 3.f});
    }

}