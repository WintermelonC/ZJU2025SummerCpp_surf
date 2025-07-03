#include "gameView.h"

GameView::GameView() 
    : m_map(nullptr),
      m_water(nullptr) {}

bool GameView::initialize(unsigned int width, unsigned int height, const std::string& title) {
    // 创建 SFML 渲染窗口
    m_window = sf::RenderWindow(sf::VideoMode({width, height}), title);
    m_view = sf::View({1280, 720}, {static_cast<float>(width), static_cast<float>(height)});
    m_window.setView(m_view);

    // 启用垂直同步
    m_window.setVerticalSyncEnabled(true);

    return true;
}

void GameView::run() {
    while (m_window.isOpen()) {
        handleEvents();
        if (m_updateCallback) {
            const sf::Vector2f mousePos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
            m_updateCallback(mousePos); // 触发回调
        }
        render();
    }
}

void GameView::handleEvents() {
    while (const std::optional event = m_window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // 处理窗口大小调整事件
            m_view.setSize(sf::Vector2f(resized->size));
            m_window.setView(m_view);
        }
    }
}

void GameView::renderBackground() {
    // 清除窗口
    m_window.clear(sf::Color(0, 192, 222));
    // 绘制水波
    m_window.draw(m_water->get());
}

void GameView::renderGameplay() {
    // 渲染背景
    renderBackground();
    // 绘制玩家
    m_window.draw(m_player->get());
}

void GameView::renderStartMenu() {
    // 渲染背景
    renderBackground();
    // 标题
    sf::Text title = renderText(
        MSYHBD_font->get(),
        "LET'S SURF",
        50,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) - sf::Vector2f{0.f, 200.f}
    );
    // 开始文字
    sf::Text startText = renderText(
        MSYHBD_font->get(),
        "开始游戏",
        35,
        sf::Color::Black,
        Config::Window::START_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );
    // 开始按钮阴影
    sf::Sprite startButtonShadow = startButton;
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 鼠标悬停按钮变化
    // 有待实现

    // 绘制玩家
    m_window.draw(m_player->get());

    // 绘制标题
    m_window.draw(title);
    // 绘制开始按钮阴影
    m_window.draw(startButtonShadow);
    // 绘制开始按钮
    m_window.draw(m_startButton->get());
    // 绘制开始图标
    m_window.draw(m_startIcon->get());
    // 绘制开始文字
    m_window.draw(startText);
}   

void GameView::renderPauseMenu() {
    renderGameplay(); // 先渲染游戏画面

    // 半透明遮罩
    sf::RectangleShape filter(static_cast<sf::Vector2f>(Config::Window::RENDER_SIZE));
    filter.setFillColor(sf::Color(255, 255, 255, 30));

    // 暂停文字
    sf::Text pausedText = renderText(
        MSYHBD_font->get(),
        "已暂停",
        75,
        sf::Color::Black,
        static_cast<sf::Vector2f>(Config::Window::RENDER_CENTER) + sf::Vector2f{0.f, -400.f},
        true
    );

    // 继续按钮阴影
    sf::Sprite continueButtonShadow = continueButton;
    continueButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    continueButtonShadow.move({0.f, 4.f});  // 向下偏移

    // 继续游戏文字
    sf::Text continueText = renderText(
        MSYHBD_font->get(),
        "继续游戏",
        35,
        sf::Color::Black,
        Config::Window::CONTINUE_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
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
    // 有待实现

    // 绘制暂停菜单
    window.draw(filter);
    window.draw(pausedText);  // 绘制暂停文字
    window.draw(continueButtonShadow);  // 绘制继续按钮阴影
    m_window.draw(m_continueButton->get()); // 绘制继续按钮
    m_window.draw(m_continueIcon->get()); // 绘制继续游戏图标
    m_window.draw(continueText);  // 绘制继续游戏文字
    window.draw(returnButtonShadow);  // 绘制返回按钮阴影
    m_window.draw(m_returnButton->get());   // 绘制返回按钮
    m_window.draw(returnText);  // 绘制返回文字
}

void GameView::renderGameOver() {
    renderGameplay(); // 先渲染游戏画面

    // 绘制游戏结束界面
}

void GameView::display() {
    m_window.display();
}

void GameView::reset() {
    // 有待实现
}

void GameView::renderText(
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