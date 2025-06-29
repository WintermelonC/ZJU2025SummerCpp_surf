#include "renderSystem.h"

void RenderSystem::render(sf::RenderWindow& window) {
    // 清除窗口
    window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口

    // 渲染逻辑
    sf::Sprite& water = EntityManager::getSprite(EntityType::water);
    sf::Sprite& player = EntityManager::getSprite(EntityType::player);

    window.draw(water);  // 绘制水
    renderRipple(window);  // 绘制水波
    renderTail(window);  // 绘制拖尾
    window.draw(player); // 绘制玩家
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
