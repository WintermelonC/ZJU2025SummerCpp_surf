#include "gameView.h"

GameView::GameView() 
    : m_water(nullptr),
      m_player(nullptr),
      m_startButton(nullptr),
      m_startIcon(nullptr),
      m_continueButton(nullptr),
      m_continueIcon(nullptr),
      m_returnButton(nullptr),
      m_score(nullptr),
      m_highScore(nullptr),
      MSYHBD_font(nullptr) {}

void GameView::run() {
    while (m_window.isOpen()) {
        float deltaTime = m_clock.restart().asSeconds();
        sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
        sf::Vector2u windowSize = m_window.getSize();
        
        handleEvents();
        
        // 触发 GameViewModel 的 update
        if (m_GameViewModelUpdateCallback) {
            m_GameViewModelUpdateCallback(windowSize);
        }
        // 触发 PlayerViewModel 的 update
        if (m_playerUpdateCallback) {
            m_playerUpdateCallback(deltaTime, mousePos, windowSize);
        }
        // 触发 ObstacleItemViewModel 的 update
        if (m_obstacleItemUpdateCallback) {
            m_obstacleItemUpdateCallback(deltaTime, *m_player->get());
        }
        // 触发 AnimationViewModel 的 update
        if (m_animationUpdateCallback) {
            m_animationUpdateCallback(deltaTime);
        }
        // 触发 SpriteViewModel 的 update
        if (m_spriteUpdateCallback) {
            m_spriteUpdateCallback(windowSize);
        }
        if (m_gameState) {
            switch (*m_gameState) {
                case Config::GameState::startMenu:
                    renderStartMenu();
                    break;
                case Config::GameState::playing:
                    renderGameplay();
                    break;
                case Config::GameState::paused:
                    renderPauseMenu();
                    break;
                case Config::GameState::gameOver:
                    renderGameOver();
                    break;
            }
        }
        
        m_window.display();
    }
}

bool GameView::initialize(unsigned int width, unsigned int height, const std::string& title) {
    // 创建 SFML 渲染窗口
    m_window = sf::RenderWindow(sf::VideoMode({width, height}), title);
    m_view = sf::View({1280, 720}, {static_cast<float>(width), static_cast<float>(height)});
    m_window.setView(m_view);

    // 启用垂直同步
    m_window.setVerticalSyncEnabled(true);

    return true;
}

void GameView::updateWindowSize(const sf::Vector2u& size) {
    // 更新窗口大小
    m_view.setSize(sf::Vector2f(size));
    m_window.setView(m_view);
}

void GameView::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            // 处理窗口关闭事件
            m_window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // 处理窗口大小调整事件
            updateWindowSize(resized->size);
        } else if (event->is<sf::Event::FocusLost>()) {
            // 窗口失去焦点事件
            if (m_onFocusLost) {
                m_onFocusLost();
            }
        } else if (event->is<sf::Event::FocusGained>()) {
            // 窗口获取焦点事件
            if (m_onFocusGained) {
                m_onFocusGained();
            }
        } else if (const auto* mouseButton = event->getIf<sf::Event::MouseButtonPressed>()) {
            // 鼠标按下
            if (mouseButton->button == sf::Mouse::Button::Right) {
                // 右键点击事件
                if (m_onMouseRightClick) {
                    m_onMouseRightClick();
                }
            } else if (mouseButton->button == sf::Mouse::Button::Left) {
                // 左键点击事件
                const auto mousePos = sf::Mouse::getPosition(m_window);
                const auto worldPos = m_window.mapPixelToCoords(mousePos);
                bool startButtonPressed = Utils::ifMouseOnButton(
                    worldPos,
                    Config::Window::START_BUTTON_POS,
                    {Config::Window::BUTTON_SIZE.x * Config::Window::START_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::START_BUTTON_SCALE.y}
                );
                bool continueButtonPressed = Utils::ifMouseOnButton(
                    worldPos,
                    Config::Window::CONTINUE_BUTTON_POS,
                    {Config::Window::BUTTON_SIZE.x * Config::Window::START_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::START_BUTTON_SCALE.y}
                );
                bool returnButtonPressed = Utils::ifMouseOnButton(
                    worldPos,
                    Config::Window::RETURN_BUTTON_POS,
                    {Config::Window::BUTTON_SIZE.x * Config::Window::START_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::START_BUTTON_SCALE.y}
                );
                bool newGameButtonPressed = Utils::ifMouseOnButton(
                    worldPos,
                    Config::Window::NEWGAME_BUTTON_POS,
                    {Config::Window::BUTTON_SIZE.x * Config::Window::NEWGAME_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::NEWGAME_BUTTON_SCALE.y}
                );
                if (m_onMouseLeftClick) {
                    m_onMouseLeftClick(startButtonPressed, continueButtonPressed, returnButtonPressed, newGameButtonPressed);
                }
            }
        } else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            // 键盘按下
            if (m_onKeyPress) {
                m_onKeyPress(*keyPressed);
            }
        }
    }
}

void GameView::renderBackground() {
    // 清除窗口
    m_window.clear(sf::Color(0, 192, 222));
    // 绘制水波
    m_window.draw(*m_water->get());
}

void GameView::renderGameplay() {
    // 渲染背景
    renderBackground();
    // 分数版阴影
    sf::Sprite scoreboardShadow = *m_scoreboard->get();
    scoreboardShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    scoreboardShadow.move({0.f, 4.f});  // 向下偏移
    // 分数
    int score = static_cast<int>(*m_score);
    sf::Text scoreText = renderText(
        *MSYHBD_font->get(),
        std::to_string(score),
        30,
        sf::Color::Black,
        m_scoreboard->get()->getPosition() + sf::Vector2f{0.f, -6.f},
        true
    );
    for (const auto& ripple : *m_ripples) {
        // 绘制水波纹
        m_window.draw(ripple.trail);
    }
    for (const auto& tail : *m_tails) {
        // 绘制尾迹
        m_window.draw(tail.trail);
    }
    // 绘制障碍物和道具
    for (const auto& sprite : *m_obstacleItemSprites) {
        m_window.draw(sprite);
    }
    // 绘制玩家
    m_window.draw(*m_player->get());
        
#ifdef DEBUG
    // 绘制障碍物道具碰撞箱
    for (const auto& bounds : *m_entityBounds) {

        sf::RectangleShape collisionBox;
        collisionBox.setSize(bounds.size);
        collisionBox.setPosition(bounds.position);
        collisionBox.setFillColor(sf::Color(175, 22, 11, 0));  // 透明
        collisionBox.setOutlineColor(sf::Color(175, 22, 11));  // 红色边框
        collisionBox.setOutlineThickness(5.0f);
        m_window.draw(collisionBox);
    }
    
    // 绘制玩家碰撞箱
    sf::FloatRect playerBounds = m_player->get()->getGlobalBounds();

    sf::Vector2f center = playerBounds.getCenter();

    sf::Vector2f newSize = {
        playerBounds.size.x * Config::Game::PLAYER_SIZE_X_COLLISION_SCALE,
        playerBounds.size.y * Config::Game::PLAYER_SIZE_Y_COLLISION_SCALE
    };

    sf::Vector2f newPosition = {
        center.x - newSize.x / 2.f + Config::Game::PLAYER_POSITION_X_OFFSET,
        center.y - newSize.y / 2.f + Config::Game::PLAYER_POSITION_Y_OFFSET
    };

    playerBounds = sf::FloatRect(newPosition, newSize);

    sf::RectangleShape playerCollisionBox;
    playerCollisionBox.setSize(playerBounds.size);
    playerCollisionBox.setPosition(playerBounds.position);
    playerCollisionBox.setFillColor(sf::Color(106, 147, 31, 0));  // 透明
    playerCollisionBox.setOutlineColor(sf::Color(106, 147, 31));  // 绿色边框
    playerCollisionBox.setOutlineThickness(5.0f);
    m_window.draw(playerCollisionBox);
#endif

    m_window.draw(scoreboardShadow);  // 绘制分数版阴影
    m_window.draw(*m_scoreboard->get());
    m_window.draw(scoreText);  // 绘制分数
    for (const auto& heart : *m_heartSprites) {
        // 绘制心形图标
        m_window.draw(heart);
    }
    for (const auto& power : *m_powerSprites) {
        // 绘制能量图标
        m_window.draw(power);
    }
}

void GameView::renderStartMenu() {
    // 渲染背景
    renderBackground();
    // 标题
    sf::Text title = renderText(
        *MSYHBD_font->get(),
        "LET'S SURF",
        50,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) - sf::Vector2f{0.f, 200.f}
    );
    // 最高分显示
    sf::Text highScoreText = renderText(
        *MSYHBD_font->get(),
        std::to_string(static_cast<int>(*m_highScore)),
        30,
        sf::Color::Black,
        m_scoreboard->get()->getPosition() + sf::Vector2f{0.f, -6.f}
    );
    // 分数版阴影
    sf::Sprite scoreboardShadow = *m_scoreboard->get();
    scoreboardShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    scoreboardShadow.move({0.f, 4.f});  // 向下偏移
    // 开始文字
    sf::Text startText = renderText(
        *MSYHBD_font->get(),
        "开始游戏",
        35,
        sf::Color::Black,
        Config::Window::START_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );
    // 开始按钮
    Utils::setSpriteColor(
        *m_startButton->get(),
        Config::Texture::BUTTON_COLOR
    );
    // 开始按钮阴影
    sf::Sprite startButtonShadow = *m_startButton->get();
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移

    mouseHoverButton(
        *m_startButton->get(), 
        startButtonShadow, 
        m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
    );
    
    // 绘制标题
    m_window.draw(title);
    // 绘制开始按钮阴影
    m_window.draw(startButtonShadow);
    // 绘制开始按钮
    m_window.draw(*m_startButton->get());
    // 绘制开始图标
    m_window.draw(*m_startIcon->get());
    // 绘制开始文字
    m_window.draw(startText);
    m_window.draw(*m_playerStartMenu->get()); // 绘制玩家开始菜单图标
    m_window.draw(scoreboardShadow);  // 绘制分数版阴影
    m_window.draw(*m_scoreboard->get());
    m_window.draw(highScoreText);
}   

void GameView::renderPauseMenu() {
    renderGameplay(); // 先渲染游戏画面

    // 半透明遮罩
    sf::RectangleShape filter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE));
    filter.setFillColor(sf::Color(255, 255, 255, 30));

    // 暂停文字
    sf::Text pausedText = renderText(
        *MSYHBD_font->get(),
        "已暂停",
        75,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) + sf::Vector2f{0.f, -400.f},
        true
    );
    // 继续按钮
    Utils::setSpriteColor(
        *m_continueButton->get(),
        Config::Texture::BUTTON_COLOR
    );
    // 继续按钮阴影
    sf::Sprite continueButtonShadow = *m_continueButton->get();
    continueButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    continueButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 继续游戏文字
    sf::Text continueText = renderText(
        *MSYHBD_font->get(),
        "继续游戏",
        35,
        sf::Color::Black,
        Config::Window::CONTINUE_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );
    // 返回按钮
    Utils::setSpriteColor(
        *m_returnButton->get(),
        Config::Texture::BUTTON_COLOR
    );
    // 返回按钮阴影
    sf::Sprite returnButtonShadow = *m_returnButton->get();
    returnButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    returnButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 返回文字
    sf::Text returnText = renderText(
        *MSYHBD_font->get(),
        "返回菜单",
        26,
        sf::Color::Black,
        Config::Window::RETURN_BUTTON_POS - sf::Vector2f{0.f, 5.f},
        true
    );

    mouseHoverButton(
        *m_continueButton->get(), 
        continueButtonShadow, 
        m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
    );
    mouseHoverButton(
        *m_returnButton->get(), 
        returnButtonShadow, 
        m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
    );

    // 绘制暂停菜单
    m_window.draw(filter);
    m_window.draw(pausedText);  // 绘制暂停文字
    m_window.draw(continueButtonShadow);  // 绘制继续按钮阴影
    m_window.draw(*m_continueButton->get()); // 绘制继续按钮
    m_window.draw(*m_continueIcon->get()); // 绘制继续游戏图标
    m_window.draw(continueText);  // 绘制继续游戏文字
    m_window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    m_window.draw(*m_returnButton->get());   // 绘制返回按钮
    m_window.draw(returnText);  // 绘制返回文字
}

void GameView::renderGameOver() {
    // 绘制游戏结束界面
    // 渲染背景
    renderBackground();
    // 分数版阴影
    sf::Sprite scoreboardShadow = *m_scoreboard->get();
    scoreboardShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    scoreboardShadow.move({0.f, 4.f});  // 向下偏移
    // 分数
    int score = static_cast<int>(*m_score);
    sf::Text scoreText = renderText(
        *MSYHBD_font->get(),
        std::to_string(score),
        30,
        sf::Color::Black,
        m_scoreboard->get()->getPosition() + sf::Vector2f{0.f, -6.f},
        true
    );
    for (const auto& ripple : *m_ripples) {
        // 绘制水波纹
        m_window.draw(ripple.trail);
    }
    for (const auto& tail : *m_tails) {
        // 绘制尾迹
        m_window.draw(tail.trail);
    }
    // 绘制障碍物和道具
    for (const auto& sprite : *m_obstacleItemSprites) {
        m_window.draw(sprite);
    }
    // 绘制玩家
    m_window.draw(*m_player->get());

    m_window.draw(scoreboardShadow);  // 绘制分数版阴影
    m_window.draw(*m_scoreboard->get());
    m_window.draw(scoreText);  // 绘制分数

    // 结束文字
    sf::Text pausedText = renderText(
        *MSYHBD_font->get(),
        "游戏结束",
        75,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) + sf::Vector2f{0.f, -400.f},
        true
    );

    // 返回按钮
    Utils::setSpriteColor(
        *m_returnButton->get(),
        Config::Texture::BUTTON_COLOR
    );
    // 返回按钮阴影
    sf::Sprite returnButtonShadow = *m_returnButton->get();
    returnButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    returnButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 返回文字
    sf::Text returnText = renderText(
        *MSYHBD_font->get(),
        "返回菜单",
        26,
        sf::Color::Black,
        Config::Window::RETURN_BUTTON_POS - sf::Vector2f{0.f, 5.f},
        true
    );

    // 新游戏按钮
    Utils::setSpriteColor(
        *m_newGameButton->get(),
        Config::Texture::BUTTON_COLOR
    );
    // 新游戏按钮阴影
    sf::Sprite newGameButtonShadow = *m_newGameButton->get();
    newGameButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    newGameButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 新游戏文字
    sf::Text newGameText = renderText(
        *MSYHBD_font->get(),
        "新游戏",
        35,
        sf::Color::Black,
        Config::Window::NEWGAME_BUTTON_POS + sf::Vector2f{20.f, -5.f},
        true
    );    

    mouseHoverButton(
        *m_newGameButton->get(), 
        newGameButtonShadow, 
        m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
    );
    mouseHoverButton(
        *m_returnButton->get(), 
        returnButtonShadow, 
        m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window))
    );

    // 绘制游戏结束界面
    m_window.draw(pausedText);  // 绘制游戏结束文字
    m_window.draw(newGameButtonShadow);  // 绘制新游戏按钮阴影
    m_window.draw(*m_newGameButton->get()); // 绘制新游戏按钮
    m_window.draw(newGameText);  // 绘制新游戏文字
    m_window.draw(*m_newGameIcon->get());
    m_window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    m_window.draw(*m_returnButton->get());   // 绘制返回按钮
    m_window.draw(returnText);  // 绘制返回文字
    
}

void GameView::display() {
    m_window.display();
}

void GameView::reset() {
    // 有待实现
}

sf::Text GameView::renderText(
    const sf::Font& font,
    const std::string& content,
    const int size,
    const sf::Color color,
    const sf::Vector2f position,
    const bool ifCenter
) {
    // 获取字体指针
    sf::Text text(font);
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

void GameView::mouseHoverButton(
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