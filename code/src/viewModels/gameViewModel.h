#pragma once

#include <SFML/Graphics.hpp>
#include "spriteViewModel.h"
#include "../models/gameModel.h"
#include "ObstacleItemViewModel.h"
#include "playerViewModel.h"
#include "../models/gameModel.h"
#include "../common/config.h"
#include "../common/notificationCenter.h"

class GameViewModel : public INotificationObserver {
public:
    GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const sf::Vector2f& mousePos, const sf::Vector2u& windowSize, const sf::Vector2f& playerVelocity);

    const float* getScore() const { return &m_gameModel.getScore(); }
    const float getDeltaTime() { return m_clock.restart().asSeconds(); }
    void usePower();
    void setGameState(GameState state) { m_gameModel.setGameState(state); }
    
    // 🔔 重置游戏 - 通过通知系统
    void resetGame();
    
    // 🔔 实现观察者接口
    void onNotification(const NotificationData& data) override;
    const GameModel& getGameModel() const { return m_gameModel; }

    std::function<void()> getFocusLostCommand();
    std::function<void()> getFocusGainedCommand();
    std::function<void(const bool& StartButtonPressed, const bool& ContinueButtonPressed,
                       const bool& ReturnButtonPressed)> getMouseLeftClickCommand();
    std::function<void(const sf::Event::KeyPressed&)> getKeyPressCommand();

private:
    void updateWater(const sf::Vector2f& playerVelocity);

private:
    const float m_waterSize = 256.f;
    const sf::Color m_buttonColor = sf::Color(195, 240, 247);
    
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    sf::Clock m_clock;
    sf::Vector2f m_waterOffset = {0, 0};
    GameModel m_gameModel;
};