#include "game.h"

Game::Game() {
    // 初始化精灵
    m_spriteViewModel = std::make_shared<SpriteViewModel>();
    m_spriteViewModel->initialize();
    m_gameViewModel = std::make_shared<GameViewModel>(m_spriteViewModel);
    m_fontViewModel = std::make_shared<FontViewModel>();
    m_fontViewModel->initialize();
}

bool Game::initialize() {
    bool success = true;
    
    // 初始化游戏视图
    success = m_gameView.initialize(Config::Window::WINDOW_SIZE.x, Config::Window::WINDOW_SIZE.y, "Surf Game");

    if (!success) {
        return false;
    }

    // 数据绑定
    m_gameView.setWater(m_spriteViewModel->getSprite(SpriteType::water));
    m_gameView.setPlayer(m_spriteViewModel->getSprite(SpriteType::player));
    m_gameView.setStartButton(m_spriteViewModel->getSprite(SpriteType::start_button));
    m_gameView.setStartIcon(m_spriteViewModel->getSprite(SpriteType::start_icon));
    m_gameView.setContinueButton(m_spriteViewModel->getSprite(SpriteType::continue_button));
    m_gameView.setReturnButton(m_spriteViewModel->getSprite(SpriteType::return_button));
    m_gameView.setFont(m_fontViewModel->getFont(Fonts::MSYHBD));
    m_gameView.setObstacleItemSprites(m_gameViewModel->getObstacleItemSprites());
    m_gameView.setScoreboard(m_spriteViewModel->getSprite(SpriteType::scoreboard));
    m_gameView.setScore(m_gameViewModel->getScore());

    return success;
}

void Game::run() {
    // 运行游戏视图
    while (m_gameView.getWindow().isOpen()) {
        // TODO: 根据游戏状态绘制不同界面
        handleEvents(m_gameView.getWindow());
        m_gameViewModel->update(m_gameView.getMousePos(), m_gameView.getWindowSize());
        m_gameView.renderGameplay();
        m_gameView.display();
    }
}

void Game::handleEvents(sf::RenderWindow& window) {
    while (const std::optional event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            // 处理窗口关闭事件
            window.close();
        } else if (const auto* resized = event->getIf<sf::Event::Resized>()) {
            // 处理窗口大小调整事件
            m_gameView.updateWindowSize(resized->size);
        } else if (const auto* mouseButton = event -> getIf<sf::Event::MouseButtonPressed>()) {
            m_gameViewModel->handleMouseEvents(*mouseButton);
        } else {
            return;
        }
    }
}
