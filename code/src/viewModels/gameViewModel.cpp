#include "viewModels/gameViewModel.h"
#include "infrastructure/utils.h"
#include "infrastructure/config.h"
#include "views/renderSystem.h"
#include <iostream>
#include <cmath>

GameViewModel::GameViewModel(std::shared_ptr<GameModel> gameModel, 
                             std::shared_ptr<PlayerModel> playerModel,
                             std::shared_ptr<IGameView> view)
    : m_gameModel(gameModel),
      m_playerModel(playerModel),
      m_view(view),
      m_playerService(std::make_shared<PlayerService>(playerModel)),
      m_inputManager(m_eventBus),
      m_lastMousePos({0.0f, 0.0f}) {
}

void GameViewModel::initialize() {
    // 设置模型事件回调
    m_gameModel->setOnStateChanged([this](GameState state) {
        // 状态改变时的处理
        if (state == GameState::Playing) {
            m_clock.restart();
        }
    });
    
    m_gameModel->setOnScoreChanged([this](float score) {
        // 分数改变时的处理（可以添加UI更新逻辑）
    });
    
    m_playerModel->setOnHPChanged([this](int hp) {
        // 生命值改变时的处理
        if (hp <= 0) {
            m_gameModel->setGameState(GameState::GameOver);
        }
    });
    
    m_playerModel->setOnPowerChanged([this](int power) {
        // 能量改变时的处理
    });
}

void GameViewModel::update(float deltaTime) {
    // 处理输入
    handleInput();
    
    // 根据游戏状态更新
    if (m_gameModel->getGameState() == GameState::Playing) {
        updatePlayer(deltaTime);
        updateGame(deltaTime);
        updateEffects(deltaTime);
    }
    
    // 更新视图数据
    m_view->updatePlayerData(*m_playerModel);
    m_view->updateGameData(*m_gameModel);
    m_view->updateWaterOffset(m_gameModel->getWaterOffset());
}

void GameViewModel::reset() {
    // 重置游戏状态
    m_gameModel->resetGame();
    
    // 重置玩家状态
    m_playerModel->resetPlayer();
    
    // 重置视图
    m_view->reset();
    
    // 重置输入管理器
    m_inputManager.reset();
    
    // 重置事件总线
    m_eventBus.clear();
    
    // 重置时钟
    m_clock.restart();
}

void GameViewModel::handleInput() {
    // 通过InputManager处理输入事件
    auto* sfmlView = dynamic_cast<SFMLGameView*>(m_view.get());
    if (sfmlView) {
        m_inputManager.processInput(sfmlView->getWindow());
        
        // 处理事件队列
        Event event;
        while (m_eventBus.poll(event)) {
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
                const auto worldPos = m_view->mapPixelToCoords(clickEvent.mousePos);
                handleMouseClick(worldPos);
            } else if (std::holds_alternative<SpacePressedEvent>(event)) {
                handleSpacePressed();
            }
        }
    }
}

void GameViewModel::render() {
    switch (m_gameModel->getGameState()) {
        case GameState::StartMenu:
            m_view->renderStartMenu();
            break;
        case GameState::Playing:
            m_view->renderGameplay();
            break;
        case GameState::Paused:
            m_view->renderPauseMenu();
            break;
        case GameState::GameOver:
            m_view->renderGameOver();
            break;
    }
    m_view->display();
}

void GameViewModel::updatePlayer(float deltaTime) {
    // 使用PlayerService更新玩家状态
    const sf::Vector2f mousePos = getMouseWorldPosition();
    m_playerService->update(deltaTime, mousePos);
}

void GameViewModel::updateGame(float deltaTime) {
    // 更新水面偏移
    m_gameModel->updateWaterOffset(m_playerModel->getVelocity());
    
    // 更新分数
    const float scoreIncrease = m_playerModel->getVelocity().y * 0.001f;
    m_gameModel->updateScore(scoreIncrease);
}

void GameViewModel::updateEffects(float deltaTime) {
    // 计算玩家速度角度
    const sf::Vector2f velocity = m_playerModel->getVelocity();
    const sf::Angle angle = m_playerModel->getAngle();
    
    // 检查是否应该生成特效
    const bool shouldRipple = shouldSpawnRipple();
    const bool shouldTail = shouldSpawnTail();
    
    // 更新特效（水波、拖尾等）
    m_view->updateEffects(deltaTime, velocity, angle, shouldRipple, shouldTail);
}

bool GameViewModel::shouldSpawnRipple() const {
    return m_playerModel->isTurn() && m_playerModel->getVelocity().y > SPEED_THRESHOLD_1;
}

bool GameViewModel::shouldSpawnTail() const {
    return m_playerModel->isPower() && m_playerModel->getVelocity().y > SPEED_THRESHOLD_2;
}

void GameViewModel::handleMouseClick(const sf::Vector2f& worldPos) {
    const GameState currentState = m_gameModel->getGameState();
    
    if (currentState == GameState::StartMenu) {
        if (isButtonClicked(worldPos, Config::Window::START_BUTTON_POS, 
            {BUTTON_SIZE.x * Config::Window::START_BUTTON_SCALE.x, BUTTON_SIZE.y * Config::Window::START_BUTTON_SCALE.y})) {
            m_gameModel->setGameState(GameState::Playing);
        }
    } else if (currentState == GameState::Paused) {
        if (isButtonClicked(worldPos, Config::Window::CONTINUE_BUTTON_POS, 
            {BUTTON_SIZE.x * Config::Window::CONTINUE_BUTTON_SCALE.x, BUTTON_SIZE.y * Config::Window::CONTINUE_BUTTON_SCALE.y})) {
            m_gameModel->setGameState(GameState::Playing);
        } else if (isButtonClicked(worldPos, Config::Window::RETURN_BUTTON_POS, 
            {BUTTON_SIZE.x * Config::Window::RETURN_BUTTON_SCALE.x, BUTTON_SIZE.y * Config::Window::RETURN_BUTTON_SCALE.y})) {
            reset();
        }
    }
}

void GameViewModel::handleSpacePressed() {
    const GameState currentState = m_gameModel->getGameState();
    
    if (currentState == GameState::Playing) {
        m_gameModel->setGameState(GameState::Paused);
    } else if (currentState == GameState::Paused) {
        m_gameModel->setGameState(GameState::Playing);
    }
}

void GameViewModel::handleRightClick() {
    if (m_gameModel->getGameState() == GameState::Playing) {
        m_playerService->usePower();
    }
}

void GameViewModel::handleWindowClose() {
    m_view->close();
}

void GameViewModel::handleWindowResize(const sf::Vector2u& size) {
    // 处理窗口大小改变
    auto* sfmlView = dynamic_cast<SFMLGameView*>(m_view.get());
    sfmlView->setViewSize(sf::Vector2f(size));
}

void GameViewModel::handleFocusLost() {
    if (m_gameModel->getGameState() == GameState::Playing) {
        m_gameModel->setGameState(GameState::Paused);
    }
}

void GameViewModel::handleFocusGained() {
    if (m_gameModel->getGameState() == GameState::Paused) {
        m_gameModel->setGameState(GameState::Playing);
    }
}

bool GameViewModel::isRunning() const {
    return m_view->isOpen();
}

sf::Vector2f GameViewModel::getMouseWorldPosition() const {
    return m_view->mapPixelToCoords(m_view->getMousePosition());
}

bool GameViewModel::isButtonClicked(const sf::Vector2f& mousePos, const sf::Vector2f& buttonPos, const sf::Vector2f& buttonSize) const {
    return Utils::ifMouseOnButton(mousePos, buttonPos, buttonSize);
}
