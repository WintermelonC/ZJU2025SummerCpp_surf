#include "gameView.h"

GameView::GameView() 
    : m_water(nullptr),
      m_player(nullptr),
      m_startButton(nullptr),
      m_startIcon(nullptr),
      m_continueButton(nullptr),
      m_continueIcon(nullptr),
      m_returnButton(nullptr),
      MSYHBD_font(nullptr) {}

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
                if (m_onMouseLeftClick) {
                    m_onMouseLeftClick(startButtonPressed, continueButtonPressed, returnButtonPressed);
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
    // 绘制玩家
    m_window.draw(*m_player->get());
    for (const auto& sprite : *m_obstacleItemSprites) {
        // 绘制障碍物和道具
        m_window.draw(sprite);
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
    // 开始文字
    sf::Text startText = renderText(
        *MSYHBD_font->get(),
        "开始游戏",
        35,
        sf::Color::Black,
        Config::Window::START_BUTTON_POS + sf::Vector2f{20.f, -6.f},
        true
    );
    // 开始按钮阴影
    sf::Sprite startButtonShadow = *m_startButton->get();
    startButtonShadow.setColor(sf::Color(0, 0, 0, 150));  // 设置阴影颜色
    startButtonShadow.move({0.f, 4.f});  // 向下偏移

    // TODO: 添加鼠标悬停按钮变化效果
    // TODO: 添加角色动画
    

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

    // TODO: 添加鼠标悬停按钮变化效果

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
    renderGameplay(); // 先渲染游戏画面

    // 绘制游戏结束界面
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