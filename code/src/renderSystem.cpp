#include "renderSystem.h"

void RenderSystem::renderBackground(sf::RenderWindow& window) {
    // 渲染背景逻辑
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite& water = EntityManager::getSprite(EntityType::water);

    window.draw(water);  // 绘制水
}

void RenderSystem::renderPlayer(
        sf::RenderWindow& window, 
        const int& HP, 
        const int& power,
        const float& score
    ) {
    // 绘制玩家
    sf::Sprite& player = EntityManager::getSprite(EntityType::player);
    window.draw(player);  // 绘制玩家精灵
    
    for (int i = 1; i <= Config::Player::PLAYER_HP; i++) {
        sf::Sprite heart = renderSprite(
            (i <= HP) ? Textures::heart_1 : Textures::heart_2,
            sf::Color::White,
            {Config::Window::RENDER_CENTER.x - HEART_X_OFFSET + i * HP_GAP, 
             Config::Window::RENDER_CENTER.y - window.getSize().y / 2 + 50},
            {HP_SCALE, HP_SCALE},
            false
        );
        window.draw(heart);  // 绘制生命值图标
    }

    for (int i = 1; i <= Config::Player::PLAYER_HP; i++) {
        sf::Sprite powerIcon = renderSprite(
            (i <= power) ? Textures::power_1 : Textures::power_2,
            sf::Color::White,
            {Config::Window::RENDER_CENTER.x + POWER_X_OFFSET + i * POWER_GAP, 
             Config::Window::RENDER_CENTER.y - window.getSize().y / 2 + 50},
            {POWER_SCALE, POWER_SCALE},
            false
        );
        window.draw(powerIcon);  // 绘制能量值图标
    }

    // 分数版
    sf::Sprite scoreboard = renderSprite(
        Textures::scoreboard,
        BUTTON_COLOR,
        {Config::Window::RENDER_CENTER.x,
         Config::Window::RENDER_CENTER.y - window.getSize().y / 2 + 50}
    );
    // 分数版阴影
    sf::Sprite scoreboardShadow = scoreboard;
    scoreboardShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    scoreboardShadow.move({0.f, 4.f});  // 向下偏移
    // 分数文本
    sf::Text scoreText = renderText(
        Fonts::MSYHBD,
        std::to_string(static_cast<int>(score)),
        30,
        sf::Color::Black,
        scoreboard.getPosition() + sf::Vector2f{0.f, -6.f},
        true
    );

    window.draw(scoreboardShadow);  // 绘制分数版阴影
    window.draw(scoreboard);  // 绘制分数板
    window.draw(scoreText);  // 绘制分数文本
}

void RenderSystem::renderEntities(sf::RenderWindow& window) {
    // 渲染所有实体
    for (const auto& entity : EntityManager::m_entities) {
        window.draw(entity->getSprite());  // 绘制实体精灵
    }
}

void RenderSystem::updateRipple(const float& dt, const sf::Vector2f& velocity, const bool& ifSpawn) {
    for (auto it = m_ripples.begin(); it != m_ripples.end(); ) {
        it -> lifetime -= dt;
        if (it -> lifetime <= 0) {
            it = m_ripples.erase(it);  // 移除过期的水波
        } else {
            it -> trail.move({-velocity.x * Config::Game::PARALLAX_FACTOR, -velocity.y * Config::Game::PARALLAX_FACTOR});  // 更新水波位置
            it -> trail.rotate(sf::degrees(Utils::randomFloat(1.f, 3.f)));  // 随机旋转水波
            sf::Color color = RIPPLE_COLOR;
            color.a = static_cast<int>(RIPPLE_ALPHA * (it -> lifetime / RIPPLE_LIFETIME));  // 渐变透明度
            it -> trail.setFillColor(color);  // 渐变透明度
            it++;
        }
    }

    spawnRipple(ifSpawn);  // 生成新的水波
}

void RenderSystem::updateTail(const float& dt, const sf::Vector2f& velocity, const sf::Angle& angle, const bool& ifSpawn) {
    for (auto it = m_tails.begin(); it != m_tails.end(); ) {
        it -> lifetime -= dt;
        if (it -> lifetime <= 0) {
            it = m_tails.erase(it);  // 移除过期的拖尾
        } else {
            it -> trail.move({-velocity.x * Config::Game::PARALLAX_FACTOR, -velocity.y * Config::Game::PARALLAX_FACTOR});  // 更新拖尾位置
            sf::Color color = TAIL_COLOR;
            color.a = static_cast<int>(TAIL_ALPHA * (it -> lifetime / TAIL_LIFETIME));  // 渐变透明度
            it -> trail.setFillColor(color);  // 渐变透明度
            it++;
        }
    }

    spawnTail(-angle, ifSpawn);  // 生成新的拖尾
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

sf::Sprite RenderSystem::renderSprite(
        const Textures& texture,
        const sf::Color color,
        const sf::Vector2f position,
        const sf::Vector2f scale,
        const bool ifSmooth,
        const bool ifCenter
    ) {
    // 获取纹理指针
    sf::Texture& texturePtr = AssetManager::getTexture(texture);
    texturePtr.setSmooth(ifSmooth);
    sf::Sprite sprite(texturePtr);
    if (ifCenter) {
        sprite.setOrigin({texturePtr.getSize().x / 2.f, texturePtr.getSize().y / 2.f});
    }
    sprite.setPosition(position);
    sprite.setScale(scale);
    sprite.setColor(color);

    return sprite;
}

void RenderSystem::renderRipple(sf::RenderWindow& window) {
    for (auto& ripple : m_ripples) {
        window.draw(ripple.trail);  // 绘制水波线段
    }
}

void RenderSystem::renderTail(sf::RenderWindow& window) {
    for (auto& tail : m_tails) {
        window.draw(tail.trail);  // 绘制拖尾线段
    }
}

void RenderSystem::spawnRipple(const bool& ifSpawn) {
    if (!ifSpawn) {
        return;
    }

    // 产生 RIPPLE_COUNT 个线段
    for (int i = 0; i < RIPPLE_COUNT; i++) {
        sf::RectangleShape line({3.f, 
                                 Utils::randomFloat(10.f, 20.f)});
        line.setRotation(sf::degrees(Utils::randomFloat(-20.f, 20.f)));
        line.setPosition({Config::Player::PLAYER_POS.x + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
                          Config::Player::PLAYER_POS.y});
        line.setFillColor(RIPPLE_COLOR);

        m_ripples.push_back({line, RIPPLE_LIFETIME});
    }
}

void RenderSystem::spawnTail(const sf::Angle& angle, const bool& ifSpawn) {
    if (!ifSpawn) {
        return;
    }

    // 产生 TAIL_COUNT 个线段
    for (int i = 0; i < TAIL_COUNT; i++) {
        sf::RectangleShape line({3.f, 
                                 Utils::randomFloat(50.f, 60.f)});
        line.setRotation(angle + sf::degrees(Utils::randomFloat(-5.f, 5.f)));
        line.setOrigin({line.getSize().x / 2.f, line.getSize().y});
        line.setPosition({Config::Player::PLAYER_POS.x + Utils::randomFloat(-Config::Player::PLAYER_SIZE.x / 3.f, Config::Player::PLAYER_SIZE.x / 3.f), 
                          Config::Player::PLAYER_POS.y});
        line.setFillColor(TAIL_COLOR);

        m_tails.push_back({line, TAIL_LIFETIME});
    }
}

void RenderSystem::renderStartMenu(sf::RenderWindow& window) {
    // 渲染开始菜单逻辑
    // 标题
    sf::Text title = renderText(
        Fonts::MSYHBD,
        "LET'S SURF",
        50,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) - sf::Vector2f{0.f, 200.f}
    );

    // 开始按钮
    sf::Sprite startButton = renderSprite(
        Textures::button,
        BUTTON_COLOR,
        Config::Window::START_BUTTON_POS,
        Config::Window::START_BUTTON_SCALE
    );

    // 开始按钮阴影
    sf::Sprite startButtonShadow = startButton;
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 开始icon
    sf::Sprite startIcon = renderSprite(
        Textures::start_icon,
        sf::Color::White,
        Config::Window::START_BUTTON_POS - sf::Vector2f{75.f, 0.f},
        {0.9f, 0.9f}
    );

    // 开始游戏文字
    sf::Text startText = renderText(
        Fonts::MSYHBD,
        "开始游戏",
        35,
        sf::Color::Black,
        Config::Window::START_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );

    renderPlayerAnimation(window);  // 渲染玩家动画

    // 鼠标悬停按钮变化
    mouseHoverButton(startButton, 
        startButtonShadow, 
        window.mapPixelToCoords(sf::Mouse::getPosition(window))
    );

    window.draw(title);
    window.draw(startButtonShadow);
    window.draw(startButton);
    window.draw(startIcon);
    window.draw(startText);
}

void RenderSystem::renderPauseMenu(sf::RenderWindow& window) {
    // 半透明遮罩
    sf::RectangleShape filter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE));
    filter.setFillColor(sf::Color(255, 255, 255, 30));

    // 暂停文字
    sf::Text pausedText = renderText(
        Fonts::MSYHBD,
        "已暂停",
        75,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) + sf::Vector2f{0.f, -400.f},
        true
    );

    // 继续按钮
    sf::Sprite continueButton = renderSprite(
        Textures::button,
        BUTTON_COLOR,
        Config::Window::CONTINUE_BUTTON_POS,
        Config::Window::CONTINUE_BUTTON_SCALE
    );

    // 继续按钮阴影
    sf::Sprite continueButtonShadow = continueButton;
    continueButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    continueButtonShadow.move({0.f, 4.f});  // 向下偏移

    // icon
    sf::Sprite continueIcon = renderSprite(
        Textures::start_icon,
        sf::Color::White,
        Config::Window::CONTINUE_BUTTON_POS - sf::Vector2f{75.f, 0.f},
        {0.9f, 0.9f}
    );

    // 继续游戏文字
    sf::Text continueText = renderText(
        Fonts::MSYHBD,
        "继续游戏",
        35,
        sf::Color::Black,
        Config::Window::CONTINUE_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );

    // 返回按钮
    sf::Sprite returnButton = renderSprite(
        Textures::button,
        BUTTON_COLOR,
        Config::Window::RETURN_BUTTON_POS,
        Config::Window::RETURN_BUTTON_SCALE
    );

    // 返回按钮阴影
    sf::Sprite returnButtonShadow = returnButton;
    returnButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    returnButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 返回文字
    sf::Text returnText = renderText(
        Fonts::MSYHBD,
        "返回菜单",
        26,
        sf::Color::Black,
        Config::Window::RETURN_BUTTON_POS - sf::Vector2f{0.f, 5.f},
        true
    );

    // 鼠标悬停按钮变化
    mouseHoverButton(continueButton, 
        continueButtonShadow, 
        window.mapPixelToCoords(sf::Mouse::getPosition(window))
    );
    mouseHoverButton(returnButton, 
        returnButtonShadow, 
        window.mapPixelToCoords(sf::Mouse::getPosition(window))
    );

    // 绘制所有元素
    window.draw(filter);
    window.draw(pausedText);  // 绘制暂停文字
    window.draw(continueButtonShadow);  // 绘制继续按钮阴影
    window.draw(continueButton);  // 绘制继续按钮
    window.draw(continueIcon);  // 绘制继续图标
    window.draw(continueText);  // 绘制继续游戏文字
    window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    window.draw(returnButton);  // 绘制返回按钮
    window.draw(returnText);  // 绘制返回菜单文字
}

void RenderSystem::renderPlayerAnimation(sf::RenderWindow& window) {
    const std::vector<Textures> animations = {
        Textures::player_left_21, Textures::player_left_22, Textures::player_left_23,
        Textures::player_left_11, Textures::player_left_12, Textures::player_left_13,
        Textures::player_center_1, Textures::player_center_2, Textures::player_center_3,
        Textures::player_right_11, Textures::player_right_12, Textures::player_right_13,
        Textures::player_right_21, Textures::player_right_22, Textures::player_right_23,
        Textures::player_right_11, Textures::player_right_12, Textures::player_right_13,
        Textures::player_left_11, Textures::player_left_12, Textures::player_left_13
    };

    const int count = animations.size();
    const float animInterval = 0.08f;  // 动画间隔时间

    // 每隔 animInterval 秒切换帧
    if (m_animClock.getElapsedTime().asSeconds() >= animInterval) {
        m_currentAnimFrame = (m_currentAnimFrame + 1) % count;  // 循环动画帧
        m_animClock.restart();  // 重置动画时钟
    }

    sf::Sprite sprite = renderSprite(
        animations[m_currentAnimFrame], 
        sf::Color::White,
        Config::Window::RENDER_CENTER,
        {2.0f, 2.0f},
        false
    );

    window.draw(sprite);  // 绘制玩家动画
}

void RenderSystem::mouseHoverButton(
        sf::Sprite& button, 
        sf::Sprite& buttonShadow, 
        const sf::Vector2f& mousePos,
        const sf::Vector2f offset,
        const sf::Color color
    ) {
    if (Utils::ifMouseOnButton(mousePos, button.getPosition(), button.getGlobalBounds().size)) {
        button.setColor(color);  // 改变颜色
        buttonShadow.move(offset);
    }
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
