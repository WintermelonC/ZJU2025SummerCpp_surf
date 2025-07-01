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

private:
    // 角度阈值
    static constexpr float ANGLE_1 = 20.0f;
    static constexpr float ANGLE_2 = 40.0f;
    
    // 物理参数
    static constexpr float MAX_SPEED = 50.0f;
    static constexpr float ACCELERATION_1 = 10.0f;
    static constexpr float ACCELERATION_2 = 50.0f;
    static constexpr float XY_SPEED_1 = 0.4f;  // Y / X 比例 1
    static constexpr float XY_SPEED_2 = 0.8f;  // Y / X 比例 2
    
    std::shared_ptr<PlayerModel> m_playerModel;
    
    // 辅助状态
    float m_powerTimer;
    float m_animationTimer;
    bool m_wasInWater;
};
