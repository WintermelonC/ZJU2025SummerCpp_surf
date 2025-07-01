#pragma once

#include <SFML/Graphics.hpp>
#include "models/playerModel.h"

// 玩家业务逻辑服务
class PlayerService {
public:
    PlayerService(std::shared_ptr<PlayerModel> playerModel);
    
    // 更新玩家状态
    void update(float deltaTime, const sf::Vector2f& mousePos);
    
    // 玩家行为
    void usePower();
    void takeDamage();
    void reset();

private:
    void updateState(const sf::Vector2f& mousePos);
    void updateTurn();
    void updatePower(float deltaTime);
    void updateYSpeed(float deltaTime);
    void updateXSpeed();
    void updateAnimation(float deltaTime);
    void updatePosition(float deltaTime);

private:
    // 角度阈值
    static constexpr float ANGLE_1 = 15.0f;
    static constexpr float ANGLE_2 = 45.0f;
    
    // 物理参数
    static constexpr float GRAVITY = 98.0f;
    static constexpr float MAX_SPEED = 100.0f;
    static constexpr float ACCELERATION = 50.0f;
    static constexpr float DECELERATION = 25.0f;
    
    std::shared_ptr<PlayerModel> m_playerModel;
    
    // 辅助状态
    float m_powerTimer;
    float m_animationTimer;
    bool m_wasInWater;
};
