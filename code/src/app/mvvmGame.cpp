#include "app/mvvmGame.h"
#include "common/assetManager.h"
#include "common/spriteManager.h"
#include <iostream>

MVVMGame::MVVMGame() {
    setupDependencies();
}

void MVVMGame::setupDependencies() {
    // 创建Models
    m_gameModel = std::make_shared<GameModel>();
    m_playerModel = std::make_shared<PlayerModel>();
    
    // 创建View
    m_gameView = std::make_shared<SFMLGameView>();
    
    // 创建ViewModel
    m_playerViewModel = std::make_shared<PlayerViewModel>(m_playerModel);
    m_gameViewModel = std::make_shared<GameViewModel>(m_gameModel, m_playerModel, m_playerViewModel, m_gameClock);

    // 创建输入管理器
    m_eventBus = std::make_shared<EventBus>();
    m_inputManager = std::make_shared<InputManager>(*m_eventBus);

    
}

void MVVMGame::initialize() {
    // 加载资源
    AssetManager::loadAssets();
    SpriteManager::loadSprites();
    
    // 初始化ViewModel
    m_gameViewModel->initialize();
    
    // std::cout << "MVVM Game initialized successfully!" << std::endl;
}

void MVVMGame::run() {
    m_gameClock.restart();
    
    while (isRunning()) {
        try {
            float deltaTime = m_gameClock.restart().asSeconds();
            // 更新ViewModel（包含模型和视图更新）
            update(deltaTime);
            
            // 渲染
            render();
            
        } catch (const std::exception& e) {
            std::cerr << "Error during game loop: " << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unknown error occurred during game loop." << std::endl;
            break;
        }
    }
}

void MVVMGame::render() {
    float deltaTime = m_gameClock.restart().asSeconds();
    const sf::Vector2f waterOffset = m_gameModel->getWaterOffset();
    const int hp = m_playerModel->getHP();
    const int power = m_playerModel->getPower();
    const float score = m_gameModel->getScore();
    switch (m_gameModel->getGameState()) {
        case GameState::StartMenu:
            m_gameView->renderStartMenu(waterOffset);
            break;
        case GameState::Playing:
            m_gameView->renderGameplay(deltaTime, waterOffset, hp, power, score);
            break;
        case GameState::Paused:
            m_gameView->renderPauseMenu(
                deltaTime, waterOffset, hp, power, score);
            break;
        case GameState::GameOver:
            m_gameView->renderGameOver();
            break;
    }
    m_gameView->display();
}

void MVVMGame::update(const float deltaTime) {
    // 处理输入
    handleInput();

    // 更新游戏状态
    if (m_gameModel->getGameState() == GameState::Playing) {
        const sf::Vector2f velocity = m_playerModel->getVelocity();
        const sf::Angle angle = m_playerModel->getAngle();
        const bool shouldRipple = shouldSpawnRipple();
        const bool shouldTail = shouldSpawnTail();
        const sf::Vector2f& mousePos = m_gameView->mapPixelToCoords(m_gameView->getMousePosition());
        m_gameViewModel->update(deltaTime, mousePos);
        // 更新水波拖尾
        m_gameView->updateEffects(deltaTime, velocity, angle, shouldRipple, shouldTail);
    }
}

void MVVMGame::reset() {
    // 重置游戏状态
    m_gameModel->resetGame();
    
    // 重置玩家状态
    m_playerModel->resetPlayer();
    
    // 重置视图
    m_gameView->reset();
    
    // 重置输入管理器
    m_inputManager->reset();
    
    // 重置事件总线
    m_eventBus->clear();
    
    // 重置时钟
    m_gameClock.restart();
}

void MVVMGame::handleInput() {
    // 通过View接口处理输入事件
    m_gameView->processInput(*m_eventBus);
        
    // 处理事件队列
    Event event;
    while (m_eventBus->poll(event)) {
        if (std::holds_alternative<WindowCloseEvent>(event)) {
            handleWindowClose();
        } else if (std::holds_alternative<WindowResizeEvent>(event)) {
            const auto& resizeEvent = std::get<WindowResizeEvent>(event);
            handleWindowResize(resizeEvent.size);
        } else if (std::holds_alternative<MouseRightClickEvent>(event)) {
            handleRightClick();
        } else if (std::holds_alternative<WindowFocusLostEvent>(event)) {
            handleFocusLost();
        } else if (std::holds_alternative<WindowFocusGainedEvent>(event)) {
            handleFocusGained();
        } else if (std::holds_alternative<MouseLeftClickEvent>(event)) {
            const auto& clickEvent = std::get<MouseLeftClickEvent>(event);
            const auto worldPos = m_gameView->mapPixelToCoords(clickEvent.mousePos);
            handleMouseClick(worldPos);
        } else if (std::holds_alternative<SpacePressedEvent>(event)) {
            handleSpacePressed();
        }
    }
}

void MVVMGame::handleWindowClose() {
    m_gameView->close();
}

void MVVMGame::handleMouseClick(const sf::Vector2f& worldPos) {
    const GameState currentState = m_gameModel->getGameState();
    
    if (currentState == GameState::StartMenu) {
        if (isButtonClicked(worldPos, Config::Window::START_BUTTON_POS, 
            {Config::Window::BUTTON_SIZE.x * Config::Window::START_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::START_BUTTON_SCALE.y})) {
            m_gameModel->setGameState(GameState::Playing);
        }
    } else if (currentState == GameState::Paused) {
        if (isButtonClicked(worldPos, Config::Window::CONTINUE_BUTTON_POS, 
            {Config::Window::BUTTON_SIZE.x * Config::Window::CONTINUE_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::CONTINUE_BUTTON_SCALE.y})) {
            m_gameModel->setGameState(GameState::Playing);
        } else if (isButtonClicked(worldPos, Config::Window::RETURN_BUTTON_POS, 
            {Config::Window::BUTTON_SIZE.x * Config::Window::RETURN_BUTTON_SCALE.x, Config::Window::BUTTON_SIZE.y * Config::Window::RETURN_BUTTON_SCALE.y})) {
            reset();
        }
    }
}

void MVVMGame::handleSpacePressed() {
    const GameState currentState = m_gameModel->getGameState();
    
    if (currentState == GameState::Playing) {
        m_gameModel->setGameState(GameState::Paused);
    } else if (currentState == GameState::Paused) {
        m_gameModel->setGameState(GameState::Playing);
    }
}

void MVVMGame::handleRightClick() {
    if (m_gameModel->getGameState() == GameState::Playing) {
        m_playerViewModel->usePower();
    }
}



void MVVMGame::handleWindowResize(const sf::Vector2u& size) {
    // 处理窗口大小改变
    m_gameView->setViewSize(sf::Vector2f(size));
}

void MVVMGame::handleFocusLost() {
    if (m_gameModel->getGameState() == GameState::Playing) {
        m_gameModel->setGameState(GameState::Paused);
    }
}

void MVVMGame::handleFocusGained() {
    if (m_gameModel->getGameState() == GameState::Paused) {
        m_gameModel->setGameState(GameState::Playing);
    }
}

bool MVVMGame::isRunning() const {
    return m_gameView->isOpen();
}

sf::Vector2f MVVMGame::getMouseWorldPosition() const {
    return m_gameView->mapPixelToCoords(m_gameView->getMousePosition());
}

bool MVVMGame::shouldSpawnRipple() const {
    return m_playerModel->isTurn() && m_playerModel->getVelocity().y > Config::Player::SPEED_THRESHOLD_1;
}

bool MVVMGame::shouldSpawnTail() const {
    return m_playerModel->isPower() && m_playerModel->getVelocity().y > Config::Player::SPEED_THRESHOLD_2;
}

bool MVVMGame::isButtonClicked(const sf::Vector2f& mousePos, const sf::Vector2f& buttonPos, const sf::Vector2f& buttonSize) const {
    return Utils::ifMouseOnButton(mousePos, buttonPos, buttonSize);
}
