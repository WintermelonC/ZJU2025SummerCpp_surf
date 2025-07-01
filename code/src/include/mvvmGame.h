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

private:
    void setupDependencies();
    
private:
    // MVVM组件
    std::shared_ptr<GameModel> m_gameModel;
    std::shared_ptr<PlayerModel> m_playerModel;
    std::shared_ptr<IGameView> m_gameView;
    std::shared_ptr<GameViewModel> m_gameViewModel;
    
    // 时钟
    sf::Clock m_gameClock;
};
