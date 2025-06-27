#include "include/game.h"

Game::Game()
    : m_window(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Surf Game"),
      m_view(sf::FloatRect({RENDER_CENTER_X, RENDER_CENTER_Y}, {WINDOW_WIDTH, WINDOW_HEIGHT})),
      m_mousePosition({0, 0}),
      m_state(GameState::Start),
      m_bgTexture(m_bgPath),
      m_bgShape({RENDER_WIDTH, RENDER_HEIGHT}),
      m_player({PLAYER_X, PLAYER_Y}) {
    // 初始化视图
    updateView();
    // 激活垂直同步
    m_window.setVerticalSyncEnabled(true);

    // 纹理重复平铺
    m_bgTexture.setRepeated(true);
    // 设置背景纹理和形状大小
    m_bgShape.setTexture(&m_bgTexture);
    m_bgShape.setTextureRect(sf::IntRect({0, 0}, {RENDER_WIDTH, RENDER_HEIGHT}));

    // 加载字体
    if (!m_fontJH.openFromFile(m_JHPath)) {
        throw std::runtime_error("Failed to load font from " + m_JHPath);
    }
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
        } else if (event->is<sf::Event::FocusLost>()) {
            // 窗口失去焦点事件
            if (m_state == GameState::Playing) {
                m_state = GameState::Paused;  // 暂停游戏
            }
        } else if (event->is<sf::Event::FocusGained>()) {
            // 窗口获得焦点事件
            if (m_state == GameState::Paused) {
                m_state = GameState::Playing;  // 恢复游戏
            }
        }
    }
}

void Game::update() {
    float dt = m_clock.restart().asSeconds();  // 获取帧时间间隔

    m_player.update(dt, sf::Mouse::getPosition(m_window), m_window.getSize());
    updateView();
    updateBackground();
}

void Game::render() {
    m_window.clear(sf::Color(0, 192, 222));  // 用纯色清除窗口
    if (m_state == GameState::Start) {
        // 绘制开始界面
        // 标题
        sf::Text title = renderText(m_fontJH, "LET'S SURF", 50, sf::Color::Black, {RENDER_CENTER_X, RENDER_CENTER_Y - 200});
        // 开始按钮
        

        m_window.draw(m_bgShape);  // 绘制背景
        m_window.draw(title);  // 绘制标题
    } else if (m_state == GameState::Paused) {
        m_window.draw(m_bgShape);  // 绘制背景
        m_window.draw(m_player.getSprite());  // 绘制玩家精灵
    } else if (m_state == GameState::Playing) {

    } else if (m_state == GameState::GameOver) {

    }
    m_window.display();  // 显示渲染结果
}

sf::Text Game::renderText(
        const sf::Font& font, 
        const std::string& content,
        const int size,
        const sf::Color color,
        const sf::Vector2f position,
        const bool ifCenter
    ) {
    sf::Text title(font);
    title.setString(content);
    title.setCharacterSize(size);
    title.setFillColor(color);
    if (ifCenter) {
        title.setOrigin(title.getLocalBounds().size / 2.0f);
    }
    title.setPosition(position);
    return title;
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
    int texWidth = m_bgTexture.getSize().x;
    int texHeight = m_bgTexture.getSize().y;
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
