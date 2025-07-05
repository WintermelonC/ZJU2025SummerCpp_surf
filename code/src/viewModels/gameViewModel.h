#pragma once

#include <SFML/Graphics.hpp>
#include "animationViewModel.h"
#include "spriteViewModel.h"
#include "obstacleItemViewModel.h"
#include "playerViewModel.h"
#include "../models/gameModel.h"
#include "../common/config.h"
#include "../common/notificationCenter.h"

class GameViewModel : public INotificationObserver {
public:
    GameViewModel(std::shared_ptr<SpriteViewModel> spriteVM);

    void update(const sf::Vector2f& mousePos);
    void usePower();
    void setGameState(GameState state) { m_gameModel.setGameState(state); }
    
    //  重置游戏 - 通过通知系统
    void resetGame();
    
    //  实现观察者接口
    void onNotification(const NotificationData& data) override;

    std::vector<sf::Sprite>& getObstacleItemSprites() { return m_ObstacleItemViewModel.getSprites(); }
    const GameModel& getGameModel() const { return m_gameModel; }

    std::function<void()> getFocusLostCommand();
    std::function<void()> getFocusGainedCommand();
    std::function<void()> getMouseRightClickCommand();
    std::function<void(const bool& StartButtonPressed, const bool& ContinueButtonPressed,
                       const bool& ReturnButtonPressed)> getMouseLeftClickCommand();
    std::function<void(const sf::Event::KeyPressed&)> getKeyPressCommand();

private:
    void updateWater();

private:
    const float m_waterSize = 256.f;
    
    std::shared_ptr<SpriteViewModel> m_spriteViewModel;
    sf::Clock m_clock;
    sf::Vector2f m_waterOffset = {0, 0};
    ObstacleItemViewModel m_ObstacleItemViewModel;
    PlayerViewModel m_playerViewModel;
    GameModel m_gameModel;
};