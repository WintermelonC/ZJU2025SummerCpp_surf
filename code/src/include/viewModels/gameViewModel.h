#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "models/gameModel.h"
#include "models/playerModel.h"
#include "views/IGameView.h"
#include "viewModels/PlayerViewModel.h"
#include "common/eventBus.h"
#include "viewModels/inputManager.h"

// 游戏ViewModel
class GameViewModel {
public:
    GameViewModel(std::shared_ptr<GameModel> gameModel, 
                  std::shared_ptr<PlayerModel> playerModel,
                  std::shared_ptr<IGameView> view);
    
    // 主要控制方法
    void initialize();
    void update(float deltaTime);
    void reset();
    void handleInput();
    void render();
    
    // 事件处理
    void handleMouseClick(const sf::Vector2f& worldPos);
    void handleSpacePressed();
    void handleRightClick();
    void handleWindowClose();
    void handleWindowResize(const sf::Vector2u& size);
    void handleFocusLost();
    void handleFocusGained();
    
    // 查询方法
    bool isRunning() const;
    sf::Vector2f getMouseWorldPosition() const;

private:
    // 更新逻辑
    void updatePlayer(float deltaTime);
    void updateGame(float deltaTime);
    void updateEffects(float deltaTime);
    
    // 状态检查
    bool shouldSpawnRipple() const;
    bool shouldSpawnTail() const;
    
    // UI交互
    bool isButtonClicked(const sf::Vector2f& mousePos, const sf::Vector2f& buttonPos, const sf::Vector2f& buttonSize) const;

private:
    // 速度阈值常量
    static constexpr float SPEED_THRESHOLD_1 = 20.0f;
    static constexpr float SPEED_THRESHOLD_2 = 40.0f;
    static constexpr sf::Vector2i BUTTON_SIZE = {180, 60};
    
    // 依赖
    std::shared_ptr<GameModel> m_gameModel;
    std::shared_ptr<PlayerModel> m_playerModel;
    std::shared_ptr<IGameView> m_view;
    std::shared_ptr<PlayerViewMoel> m_PlayerViewMoel;
    
    // 输入和事件
    EventBus m_eventBus;
    InputManager m_inputManager;
    
    // 游戏状态
    sf::Clock m_clock;
    sf::Vector2f m_lastMousePos;
};
