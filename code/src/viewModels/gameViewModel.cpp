#include "viewModels/gameViewModel.h"
#include "common/utils.h"
#include "common/config.h"
#include <iostream>
#include <cmath>

GameViewModel::GameViewModel(std::shared_ptr<GameModel> gameModel, 
                             std::shared_ptr<PlayerModel> playerModel,
                             std::shared_ptr<PlayerViewModel> playerViewModel,
                             sf::Clock& clock
                            )
    : m_gameModel(gameModel),
      m_playerModel(playerModel),
      m_playerViewModel(playerViewModel),
      m_clock(clock)
    {}

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

void GameViewModel::update(
    float deltaTime, 
    const sf::Vector2f& mousePos
) {
    // 根据游戏状态更新
    if (m_gameModel->getGameState() == GameState::Playing) {
        updatePlayer(deltaTime, mousePos);
        updateGame(deltaTime);
    }
}

void GameViewModel::updatePlayer(float deltaTime, const sf::Vector2f& mousePos) {
    // 使用PlayerViewModel更新玩家状态
    m_playerViewModel->update(deltaTime, mousePos);
}

void GameViewModel::updateGame(float deltaTime) {
    // 更新水面偏移
    m_gameModel->updateWaterOffset(m_playerModel->getVelocity());
    
    // 更新分数
    const float scoreIncrease = m_playerModel->getVelocity().y * 0.001f;
    m_gameModel->updateScore(scoreIncrease);
}

