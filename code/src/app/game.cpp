#include "game.h"

Game::Game() {
    // 初始化精灵
    m_spriteViewModel = std::make_shared<SpriteViewModel>();
    m_spriteViewModel->initialize();
    m_gameViewModel = std::make_shared<GameViewModel>(m_spriteViewModel);
}

bool Game::initialize() {
    bool success = true;
    
    // 初始化游戏视图
    success = m_gameView.initialize(720, 1280, "Surf Game");

    if (!success) {
        return false;
    }

    // 数据绑定
    m_gameView.setMap(&m_gameViewModel->getMap());
    m_gameView.setWater(m_spriteViewModel->getSprite(SpriteType::water));
    m_gameView.setPlayer(m_spriteViewModel->getSprite(SpriteType::player));

    // 绑定函数回调
    m_gameView.setUpdateCallback([this](const sf::Vector2f& mousePos) {
        // 在这里可以调用 GameViewModel 的更新方法
        m_gameViewModel->update(mousePos);
    });

    return success;
}

void Game::run() {
    // 运行游戏视图
    m_gameView.run();
}
