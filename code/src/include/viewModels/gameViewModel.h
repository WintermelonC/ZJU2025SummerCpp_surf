#pragma once

#include <memory>
#include <SFML/Graphics.hpp>
#include "models/gameModel.h"
#include "models/playerModel.h"
#include "viewModels/playerViewModel.h"
#include "common/eventBus.h"
#include "viewModels/inputManager.h"

// 游戏ViewModel
class GameViewModel {
public:
    GameViewModel(std::shared_ptr<GameModel> gameModel, 
                  std::shared_ptr<PlayerModel> playerModel,
                  std::shared_ptr<PlayerViewModel> playerViewModel,
                  sf::Clock& clock
    );
    
    // 主要控制方法
    void initialize();
    void update(
        float deltaTime, 
        const sf::Vector2f& mousePos
    );
    void reset();
    
    // 查询方法
    bool isRunning() const;
    sf::Vector2f getMouseWorldPosition() const;

private:
    // 更新逻辑
    void updatePlayer(float deltaTime, const sf::Vector2f& mousePos);
    void updateGame(float deltaTime);
    
    // 状态检查
    bool shouldSpawnRipple() const;
    bool shouldSpawnTail() const;
    
    // UI交互
    bool isButtonClicked(const sf::Vector2f& mousePos, const sf::Vector2f& buttonPos, const sf::Vector2f& buttonSize) const;

private:
    // 依赖
    std::shared_ptr<GameModel> m_gameModel;
    std::shared_ptr<PlayerModel> m_playerModel;
    std::shared_ptr<PlayerViewModel> m_playerViewModel;
    
    // 游戏状态
    sf::Clock& m_clock;
};
