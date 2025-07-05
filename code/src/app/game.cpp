#include "game.h"

Game::Game() {
    // 初始化精灵
    m_spriteViewModel = std::make_shared<SpriteViewModel>();
    m_spriteViewModel->initialize();
    m_gameViewModel = std::make_shared<GameViewModel>(m_spriteViewModel);
    m_fontViewModel = std::make_shared<FontViewModel>();
    m_fontViewModel->initialize();
    m_ObstacleItemViewModel = std::make_shared<ObstacleItemViewModel>(m_spriteViewModel);
    m_playerViewModel = std::make_shared<PlayerViewModel>(m_spriteViewModel);

     // 订阅通知 - 在所有对象创建完成后
    m_gameViewModel->subscribeToNotifications();
    m_playerViewModel->subscribeToNotifications();
    m_ObstacleItemViewModel->subscribeToNotifications();
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
    m_gameView.setContinueIcon(m_spriteViewModel->getSprite(SpriteType::continue_icon));
    m_gameView.setReturnButton(m_spriteViewModel->getSprite(SpriteType::return_button));
    m_gameView.setFont(m_fontViewModel->getFont(Fonts::MSYHBD));
    m_gameView.setObstacleItemSprites(m_ObstacleItemViewModel->getSprites());
    m_gameView.setScoreboard(m_spriteViewModel->getSprite(SpriteType::scoreboard));
    m_gameView.setScore(m_gameViewModel->getScore());
    m_gameView.setGameState(&m_gameViewModel->getGameModel().getGameState());
    m_gameView.setRipples(&m_playerViewModel->getRipples());
    m_gameView.setTails(&m_playerViewModel->getTails());
    m_gameViewModel->setPlayerVelocity(&m_playerViewModel->getPlayerVelocity());
    m_playerViewModel->setGameState(&m_gameViewModel->getGameModel().getGameState());
    m_ObstacleItemViewModel->setPlayerVelocity(&m_playerViewModel->getPlayerVelocity());
    m_ObstacleItemViewModel->setPlayerState(&m_playerViewModel->getPlayerState());
    m_ObstacleItemViewModel->setGameState(&m_gameViewModel->getGameModel().getGameState());

    // 设置事件回调
    // 设置焦点丢失回调
    m_gameView.setOnFocusLost(m_gameViewModel->getFocusLostCommand());
    // 设置焦点获取回调
    m_gameView.setOnFocusGained(m_gameViewModel->getFocusGainedCommand());
    // 设置鼠标右键点击回调
    m_gameView.setOnMouseRightClick(m_playerViewModel->getMouseRightClickCommand());
    // 设置鼠标左键点击回调
    m_gameView.setOnMouseLeftClick(m_gameViewModel->getMouseLeftClickCommand());
    // 设置键盘按下回调
    m_gameView.setOnKeyPress(m_gameViewModel->getKeyPressCommand());
    m_gameView.setUpdateCallback(m_gameViewModel->getUpdateCommand());
    m_gameView.setPlayerUpdateCallback(m_playerViewModel->getUpdateCommand());
    m_gameView.setObstacleItemUpdateCallback(m_ObstacleItemViewModel->getUpdateCommand());

    return success;
}

void Game::run() {
    // 运行游戏视图
    m_gameView.run();
}
