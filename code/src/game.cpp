#include "include/game.h"

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Surf Game"),
      m_view(sf::FloatRect({RENDER_CENTER_X, RENDER_CENTER_Y}, {WINDOW_WIDTH, WINDOW_HEIGHT})),
      m_state(GameState::Start),
      m_bgShape({RENDER_WIDTH, RENDER_HEIGHT}),
      m_player({PLAYER_X, PLAYER_Y}) {
    // 初始化视图
    updateView();
    // 激活垂直同步
    m_window.setVerticalSyncEnabled(true);

    // 设置背景纹理和形状大小
    sf::Texture* bgTexture = Utils::getTexture(Textures::water);
    bgTexture -> setRepeated(true);  // 设置纹理重复
    m_bgShape.setTexture(bgTexture);
    m_bgShape.setTextureRect(sf::IntRect({0, 0}, {RENDER_WIDTH, RENDER_HEIGHT}));

    m_player.initial();  // 初始化玩家状态
}

void Game::run() {
    while (m_window.isOpen()) {
        handleEvents();
        if (m_state == GameState::Playing) {
            update();
        }
        render();
    }
}

void Game::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event -> is<sf::Event::Closed>()) {
            // 关闭窗口事件
            m_window.close();
        } else if (const auto* resized = event -> getIf<sf::Event::Resized>()) {
            // 窗口大小调整事件
            m_view.setSize(sf::Vector2f(resized -> size));
            updateView();
        } else if (event -> is<sf::Event::FocusLost>()) {
            // 窗口失去焦点事件
            if (m_state == GameState::Playing) {
                m_state = GameState::Paused;  // 暂停游戏
            }
        } else if (event -> is<sf::Event::FocusGained>()) {
            // 窗口获得焦点事件
            if (m_state == GameState::Paused) {
                m_state = GameState::Playing;  // 恢复游戏
            }
        } else if (const auto* mouseButton = event -> getIf<sf::Event::MouseButtonPressed>()) {
            // 鼠标点击
            if (mouseButton -> button == sf::Mouse::Button::Left) {
                // 左键点击
                handleMouseLeftClick({static_cast<float>(mouseButton -> position.x), static_cast<float>(mouseButton -> position.y)});
            }
        }

        if (const auto* keyPressed = event -> getIf<sf::Event::KeyPressed>()) {
            // 键盘按下事件
            if (keyPressed -> code == sf::Keyboard::Key::Space) {
                // 空格
                if (m_state == GameState::Playing) {
                    m_state = GameState::Paused;  // 暂停游戏
                } else if (m_state == GameState::Paused) {
                    m_state = GameState::Playing;  // 恢复游戏
                } else if (m_state == GameState::Start) {
                    m_state = GameState::Playing;  // 从开始状态进入游戏
                }
            }
        }
    }
}

void Game::handleMouseLeftClick(const sf::Vector2f& mousePos) {
    // 将鼠标坐标转换为视图坐标
    sf::Vector2f worldPos = m_window.mapPixelToCoords(sf::Vector2i(mousePos));
    
    // 检查是否点击了开始按钮
    if (m_state == GameState::Start) {
        // 开始按钮的位置和大小
        float buttonX = START_BUTTON_X;
        float buttonY = START_BUTTON_Y;
        float buttonWidth = START_BUTTON_WIDTH * START_BUTTON_SCALE;  // 考虑缩放
        float buttonHeight = START_BUTTON_HEIGHT * START_BUTTON_SCALE;
        // 检查点击是否在按钮范围内
        if (Utils::ifMouseOnButton(worldPos, buttonX, buttonY, buttonWidth, buttonHeight)) {
            m_state = GameState::Playing;  // 切换到游戏状态
            m_clock.restart();  // 重置时钟
        }
    } else if (m_state == GameState::Paused) {
        // 继续按钮的位置和大小
        float buttonX = CONTINUE_BUTTON_X;
        float buttonY = CONTINUE_BUTTON_Y;
        float buttonWidth = CONTINUE_BUTTON_WIDTH * CONTINUE_BUTTON_SCALE;  // 考虑缩放
        float buttonHeight = CONTINUE_BUTTON_HEIGHT * CONTINUE_BUTTON_SCALE;
        // 检查点击是否在按钮范围内
        if (Utils::ifMouseOnButton(worldPos, buttonX, buttonY, buttonWidth, buttonHeight)) {
            m_state = GameState::Playing;  // 切换到游戏状态
            m_clock.restart();  // 重置时钟
        }
        buttonX = RETURN_BUTTON_X;
        buttonY = RETURN_BUTTON_Y;
        buttonWidth = RETURN_BUTTON_WIDTH * RETURN_BUTTON_SCALE;  // 考虑缩放
        buttonHeight = RETURN_BUTTON_HEIGHT * RETURN_BUTTON_SCALE;
        // 检查点击是否在返回按钮范围内
        if (Utils::ifMouseOnButton(worldPos, buttonX, buttonY, buttonWidth, buttonHeight)) {
            m_state = GameState::Start;  // 切换到开始状态
            m_player.initial();
            m_clock.restart();  // 重置时钟
        }
    }
}

void Game::update() {
    float dt = m_clock.restart().asSeconds();  // 获取帧时间间隔

    m_player.update(dt, m_window);
    updateView();
    updateBackground();
}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口
    if (m_state == GameState::Start) {
        renderStartMenu();
        renderPlayerState();
    } else if (m_state == GameState::Paused) {
        renderPausedMenu();
        renderPlayerState();
    } else if (m_state == GameState::Playing) {
        renderPlayerState();
        m_window.draw(m_bgShape);  // 绘制背景
        m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    } else if (m_state == GameState::GameOver) {

    }
    m_window.display();  // 显示渲染结果
}

void Game::renderStartMenu() {
    // 标题
    sf::Text title = Utils::renderText(
        Fonts::MSJHBD, 
        "LET'S SURF", 
        50, 
        sf::Color::Black, 
        {RENDER_CENTER_X, RENDER_CENTER_Y - 200}
    );
    // 开始按钮
    sf::Sprite startButton = Utils::renderSprite(
        Textures::start_button,
        sf::Color(195, 240, 247),
        {START_BUTTON_X, START_BUTTON_Y},
        {START_BUTTON_SCALE, START_BUTTON_SCALE}
    );
    // 开始按钮阴影
    sf::Sprite startButtonShadow = startButton;
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移
    // 开始 icon
    sf::Sprite startIcon = Utils::renderSprite(
        Textures::start_icon,
        sf::Color::White,
        {START_BUTTON_X - 75, START_BUTTON_Y},
        {0.9f, 0.9f}
    );
    // 开始游戏文字
    sf::Text startText = Utils::renderText(
        Fonts::almmdfdk, 
        "开始游戏", 
        35, 
        sf::Color::Black, 
        {START_BUTTON_X + 20, START_BUTTON_Y - 6}, 
        true
    );
    // 鼠标悬停变化
    Utils::mouseHoverButton(
        startButton, 
        startButtonShadow, 
        m_window,
        {0.f, 0.3f}
    );

    // 绘制人物动画
    renderPlayerAnimation();
    m_window.draw(m_bgShape);  // 绘制背景
    m_window.draw(title);  // 绘制标题
    m_window.draw(startButtonShadow);  // 绘制开始按钮阴影
    m_window.draw(startButton);  // 绘制开始按钮
    m_window.draw(startIcon);  // 绘制开始图标
    m_window.draw(startText);  // 绘制开始游戏文字
}

void Game::renderPausedMenu() {
    // 半透明遮罩
    sf::RectangleShape filter({RENDER_WIDTH, RENDER_HEIGHT});
    filter.setFillColor(sf::Color(255, 255, 255, 30));
    // 暂停文字
    sf::Text pausedText = Utils::renderText(
        Fonts::almmdfdk, 
        "已暂停", 
        75, 
        sf::Color::Black, 
        {RENDER_CENTER_X, RENDER_CENTER_Y - 400}, 
        true
    );
    // 继续游戏按钮
    sf::Sprite continueButton = Utils::renderSprite(
        Textures::start_button,
        sf::Color(195, 240, 247),
        {CONTINUE_BUTTON_X, CONTINUE_BUTTON_Y},
        {CONTINUE_BUTTON_SCALE, CONTINUE_BUTTON_SCALE}
    );
    // 按钮阴影
    sf::Sprite continueButtonShadow = continueButton;
    continueButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    continueButtonShadow.move({0.f, 4.f});  // 向下偏移
    // icon
    sf::Sprite continueIcon = Utils::renderSprite(
        Textures::start_icon,
        sf::Color::White,
        {CONTINUE_BUTTON_X - 75, CONTINUE_BUTTON_Y},
        {0.9f, 0.9f}
    );
    // 继续游戏文字
    sf::Text continueText = Utils::renderText(
        Fonts::almmdfdk, 
        "继续游戏", 
        35, 
        sf::Color::Black, 
        {CONTINUE_BUTTON_X + 20, CONTINUE_BUTTON_Y - 6}, 
        true
    );
    // 返回菜单按钮
    sf::Sprite returnButton = Utils::renderSprite(
        Textures::start_button,
        sf::Color(195, 240, 247),
        {RETURN_BUTTON_X, RETURN_BUTTON_Y},
        {RETURN_BUTTON_SCALE, RETURN_BUTTON_SCALE}
    );
    // 返回按钮阴影
    sf::Sprite returnButtonShadow = returnButton;
    returnButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    returnButtonShadow.move({0.f, 4.f});  // 向下偏移
    // 返回按钮文本
    sf::Text returnText = Utils::renderText(
        Fonts::almmdfdk, 
        "返回菜单", 
        26, 
        sf::Color::Black, 
        {RETURN_BUTTON_X, RETURN_BUTTON_Y - 5}, 
        true
    );
    // 鼠标悬停变化
    Utils::mouseHoverButton(
        continueButton, 
        continueButtonShadow, 
        m_window,
        {0.f, 0.3f}
    );
    Utils::mouseHoverButton(
        returnButton, 
        returnButtonShadow, 
        m_window,
        {0.f, 0.3f}
    );

    m_window.draw(m_bgShape);  // 绘制背景
    m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    m_window.draw(filter);  // 绘制半透明遮罩
    m_window.draw(pausedText);  // 绘制暂停文字
    m_window.draw(continueButtonShadow);  // 绘制继续按钮阴影
    m_window.draw(continueButton);  // 绘制继续按钮
    m_window.draw(continueIcon);  // 绘制继续图标
    m_window.draw(continueText);  // 绘制继续游戏文字
    m_window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    m_window.draw(returnButton);  // 绘制返回按钮
    m_window.draw(returnText);  // 绘制返回菜单文字
}

void Game::renderPlayerAnimation() {
    const std::vector<Textures> paths = m_player.getPaths();
    const int count = paths.size();
    const float animInterval = 0.08f;  // 动画间隔时间

    // 每隔 animInterval 秒切换帧
    if (m_animClock.getElapsedTime().asSeconds() >= animInterval) {
        m_currentAnimFrame = (m_currentAnimFrame + 1) % count;  // 循环动画帧
        m_animClock.restart();  // 重置动画时钟
    }

    sf::Sprite sprite = Utils::renderSprite(
        paths[m_currentAnimFrame], 
        sf::Color::White,
        {RENDER_CENTER_X, RENDER_CENTER_Y},
        {2.0f, 2.0f},
        false
    );
    m_window.draw(sprite);  // 绘制玩家动画
}

void Game::updateView() {
    // 更新视图中心为渲染中心
    m_view.setCenter({RENDER_CENTER_X, RENDER_CENTER_Y});
    m_window.setView(m_view);
}

void Game::updateBackground() {
    // 根据玩家移动方向反向移动背景
    m_offsetX += m_player.getVelocity().x * PARALLAX_FACTOR;
    m_offsetY += m_player.getVelocity().y * PARALLAX_FACTOR;
    // 保证 m_offsetX m_offsetY 在纹理高度范围内循环
    int texWidth = m_bgShape.getTexture() -> getSize().x;
    int texHeight = m_bgShape.getTexture() -> getSize().y;
    if (m_offsetX < 0) {
        m_offsetX += texWidth;
    } else if (m_offsetX >= texWidth) {
        m_offsetX -= texWidth;
    }
    if (m_offsetY < 0) {
        m_offsetY += texHeight;
    } else if (m_offsetY >= texHeight) {
        m_offsetY -= texHeight;
    }
    m_bgShape.setTextureRect(sf::IntRect({static_cast<int>(m_offsetX), static_cast<int>(m_offsetY)}, {RENDER_WIDTH, RENDER_HEIGHT}));
}

void Game::renderPlayerState() {
    const int hp = m_player.getHP();
    const int power = m_player.getPower();
    std::vector<sf::Sprite> hearts;
    std::vector<sf::Sprite> powers;

    for (int i = 1; i <= PLAYER_HP; i++) {
        sf::Sprite heart = Utils::renderSprite(
            (i <= hp) ? Textures::heart_1 : Textures::heart_2,
            sf::Color::White,
            {static_cast<float>(RENDER_CENTER_X - HEART_X_OFFSET + i * 50), 
             static_cast<float>(RENDER_CENTER_Y - m_window.getSize().y / 2 + 50)},
            {2.0f, 2.0f},
            false
        );
        sf::Sprite powerIcon = Utils::renderSprite(
            (i <= power) ? Textures::power_1 : Textures::power_2,
            sf::Color::White,
            {static_cast<float>(RENDER_CENTER_X + POWER_X_OFFSET + i * 50), 
             static_cast<float>(RENDER_CENTER_Y - m_window.getSize().y / 2 + 50)},
            {2.0f, 2.0f},
            false
        );
        hearts.push_back(heart);
        powers.push_back(powerIcon);
    }

    for (const auto& heart : hearts) {
        m_window.draw(heart);  // 绘制生命值图标
    }
    for (const auto& power : powers) {
        m_window.draw(power);  // 绘制能量值图标
    }
}
