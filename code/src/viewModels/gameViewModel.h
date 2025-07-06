#pragma once

#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../common/notificationCenter.h"
#include "../models/gameModel.h"

class GameViewModel : public INotificationObserver, public std::enable_shared_from_this<GameViewModel> {
public:
    GameViewModel();

    // 主要更新方法
    void update(const sf::Vector2u& windowSize);

    // Getter 方法
    const float* getScore() const { return &m_gameModel.getScore(); }
    const GameModel& getGameModel() const { return m_gameModel; }
    const Config::GameState& getGameState() const { return m_gameModel.getGameState(); }
    const sf::Vector2f& getWaterOffset() const { return m_waterOffset; }

    // 游戏控制方法
    void usePower();
    void setGameState(Config::GameState state) { m_gameModel.setGameState(state); }
    void resetGame();  // 重置游戏 - 通过通知系统
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;

    // 在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();

    // Setter 方法
    void setPlayerVelocity(const sf::Vector2f* velocity) { m_playerVelocity = velocity; }
    void setPlayerHP(const int& playerHP) { m_playerHP = &playerHP; }

    // 获取回调方法
    Config::FocusLostCallback getFocusLostCommand();
    Config::FocusGainedCallback getFocusGainedCommand();
    Config::MouseLeftClickCallback getMouseLeftClickCommand();
    Config::KeyPressCallback getKeyPressCommand();
    Config::GameUpdateCallback getUpdateCommand();

private:
    void updateWater(const sf::Vector2f& playerVelocity);

private:
    // 游戏配置常量
    const float m_waterSize = 256.f;
    const sf::Color m_buttonColor = sf::Color(195, 240, 247);
    
    // 游戏状态
    sf::Vector2f m_waterOffset = {0, 0};
    GameModel m_gameModel;
    const int* m_playerHP;  // 玩家当前生命值
    
    // 外部依赖
    const sf::Vector2f* m_playerVelocity;
};