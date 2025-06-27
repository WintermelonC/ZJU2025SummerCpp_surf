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
                if (m_state == GameState::Start) {
                    handleMouseClick({static_cast<float>(mouseButton -> position.x), static_cast<float>(mouseButton -> position.y)});
                }
            }
        }
    }
}

void Game::handleMouseClick(const sf::Vector2f& mousePos) {
    // 将鼠标坐标转换为视图坐标
    sf::Vector2f worldPos = m_window.mapPixelToCoords(sf::Vector2i(mousePos));
    
    // 检查是否点击了开始按钮
    if (m_state == GameState::Start) {
        // 开始按钮的位置和大小
        float buttonX = RENDER_CENTER_X;
        float buttonY = RENDER_CENTER_Y + 200;
        float buttonWidth = 180.0f * START_BUTTON_SCALE;  // 考虑缩放
        float buttonHeight = 60.0f * START_BUTTON_SCALE;
        
        // 检查点击是否在按钮范围内
        if (worldPos.x >= buttonX - buttonWidth / 2 && 
            worldPos.x <= buttonX + buttonWidth / 2 &&
            worldPos.y >= buttonY - buttonHeight / 2 && 
            worldPos.y <= buttonY + buttonHeight / 2) {
            // 点击了开始按钮，切换到游戏状态
            m_state = GameState::Playing;
            m_clock.restart();  // 重置游戏时钟
        }
    }
}

void Game::update() {
    float dt = m_clock.restart().asSeconds();  // 获取帧时间间隔

    m_player.update(dt, sf::Mouse::getPosition(m_window), m_window);
    updateView();
    updateBackground();
}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口
    if (m_state == GameState::Start) {
        renderStartMenu();
    } else if (m_state == GameState::Paused) {
        m_window.draw(m_bgShape);  // 绘制背景
        m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    } else if (m_state == GameState::Playing) {
        m_window.draw(m_bgShape);  // 绘制背景
        m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    } else if (m_state == GameState::GameOver) {

    }
    m_window.display();  // 显示渲染结果
}

void Game::renderStartMenu() {
    // 标题
    sf::Text title = Utils::renderText(Fonts::MSJHBD, "LET'S SURF", 50, sf::Color::Black, {RENDER_CENTER_X, RENDER_CENTER_Y - 200});
    // 开始按钮
    sf::Sprite startButton = Utils::renderSprite(
        Textures::start_button,
        sf::Color(195, 240, 247),
        {RENDER_CENTER_X, RENDER_CENTER_Y + 200},
        {START_BUTTON_SCALE, START_BUTTON_SCALE},
        true,
        true
    );
    // 开始按钮阴影
    sf::Sprite startButtonShadow = startButton;
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移
    // 开始 icon
    sf::Sprite startIcon = Utils::renderSprite(
        Textures::start_icon,
        sf::Color::White,
        {RENDER_CENTER_X - 75, RENDER_CENTER_Y + 200},
        {0.9f, 0.9f},
        true,
        true
    );
    // 开始游戏文字
    sf::Text startText = Utils::renderText(Fonts::almmdfdk, "开始游戏", 35, sf::Color::Black, {RENDER_CENTER_X + 20, RENDER_CENTER_Y + 195}, true, true);
    // 绘制人物动画
    renderPlayerAnimation();

    // 鼠标悬停变化
    sf::Vector2i mousePos = sf::Mouse::getPosition(m_window);
    sf::Vector2f worldPos = m_window.mapPixelToCoords(mousePos);
    float buttonX = startButton.getPosition().x;
    float buttonY = startButton.getPosition().y;
    float buttonWidth = START_BUTTON_WIDTH * START_BUTTON_SCALE;
    float buttonHeight = START_BUTTON_HEIGHT * START_BUTTON_SCALE;
    if (worldPos.x >= buttonX - buttonWidth / 2 && 
        worldPos.x <= buttonX + buttonWidth / 2 &&
        worldPos.y >= buttonY - buttonHeight / 2 && 
        worldPos.y <= buttonY + buttonHeight / 2) {
        // 鼠标悬停在按钮上，改变按钮颜色
        startButton.setColor(sf::Color(255, 255, 255));  // 改变颜色
        startButtonShadow.move({0.f, 3.f});
    }

    m_window.draw(m_bgShape);  // 绘制背景
    m_window.draw(title);  // 绘制标题
    m_window.draw(startButtonShadow);  // 绘制开始按钮阴影
    m_window.draw(startButton);  // 绘制开始按钮
    m_window.draw(startIcon);  // 绘制开始图标
    m_window.draw(startText);  // 绘制开始游戏文字
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
        true,
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
