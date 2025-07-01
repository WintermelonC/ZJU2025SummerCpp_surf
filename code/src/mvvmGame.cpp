#include "mvvmGame.h"
#include "infrastructure/assetManager.h"
#include "views/entityManager.h"
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
    m_gameViewModel = std::make_shared<GameViewModel>(m_gameModel, m_playerModel, m_gameView);
}

void MVVMGame::initialize() {
    // 加载资源
    AssetManager::loadAssets();
    EntityManager::loadSprites();
    
    // 初始化ViewModel
    m_gameViewModel->initialize();
    
    // std::cout << "MVVM Game initialized successfully!" << std::endl;
}

void MVVMGame::run() {
    m_gameClock.restart();
    
    while (m_gameViewModel->isRunning()) {
        const float deltaTime = m_gameClock.restart().asSeconds();
        
        try {
            // 更新ViewModel（包含模型和视图更新）
            m_gameViewModel->update(deltaTime);
            
            // 渲染
            m_gameViewModel->render();
            
        } catch (const std::exception& e) {
            std::cerr << "Error during game loop: " << e.what() << std::endl;
            break;
        } catch (...) {
            std::cerr << "Unknown error occurred during game loop." << std::endl;
            break;
        }
    }
}
