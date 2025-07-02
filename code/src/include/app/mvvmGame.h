#pragma once

#include <memory>
#include "models/gameModel.h"
#include "models/playerModel.h"
#include "views/gameView.h"
#include "viewModels/gameViewModel.h"

// MVVM架构的主游戏类
class MVVMGame {
public:
    MVVMGame();
    ~MVVMGame() = default;
    
    // 主要方法
    void initialize();
    void run();
    void reset();
    void update(const float deltaTime);
    void render();

private:
    // 输入处理
    void handleInput();
    void handleMouseClick(const sf::Vector2f& worldPos);
    void handleSpacePressed();
    void handleRightClick();
    void handleWindowResize(const sf::Vector2u& size);
    void handleFocusLost();
    void handleFocusGained();
    void handleWindowClose();

    void setupDependencies();

    bool isRunning() const;
    sf::Vector2f getMouseWorldPosition() const;
    bool isButtonClicked(const sf::Vector2f& mousePos, const sf::Vector2f& buttonPos, const sf::Vector2f& buttonSize) const;
    bool shouldSpawnRipple() const;
    bool shouldSpawnTail() const;
    
private:
    // MVVM组件
    std::shared_ptr<GameModel> m_gameModel;
    std::shared_ptr<PlayerModel> m_playerModel;
    std::shared_ptr<SFMLGameView> m_gameView;
    std::shared_ptr<GameViewModel> m_gameViewModel;
    std::shared_ptr<PlayerViewModel> m_playerViewModel;
    std::shared_ptr<InputManager> m_inputManager;
    std::shared_ptr<EventBus> m_eventBus;
    
    // 时钟
    sf::Clock m_gameClock;
};
