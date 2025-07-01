#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "infrastructure/config.h"

enum class PlayerState {
    Left2, Left1, Center, Right1, Right2, Stop
};

// 玩家数据模型
class PlayerModel {
public:
    PlayerModel();
    
    // 获取器
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getVelocity() const { return m_velocity; }
    sf::Angle getAngle() const { return m_angle; }
    PlayerState getState() const { return m_state; }
    int getHP() const { return m_hp; }
    int getPower() const { return m_power; }
    bool isTurn() const { return m_isTurn; }
    bool isPower() const { return m_isPower; }
    
    // 设置器
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setAngle(sf::Angle angle) { m_angle = angle; }
    void setState(PlayerState state) { m_state = state; }
    void setTurn(bool isTurn) { m_isTurn = isTurn; }
    void setPower(bool isPower) { m_isPower = isPower; }
    
    // 业务逻辑
    void usePower();
    void takeDamage();
    void resetPlayer();
    
    // 事件回调
    void setOnHPChanged(std::function<void(int)> callback) { 
        m_onHPChanged = callback; 
    }
    void setOnPowerChanged(std::function<void(int)> callback) { 
        m_onPowerChanged = callback; 
    }

private:
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Angle m_angle;
    PlayerState m_state;
    int m_hp;
    int m_power;
    bool m_isTurn;
    bool m_isPower;
    
    // 事件回调
    std::function<void(int)> m_onHPChanged;
    std::function<void(int)> m_onPowerChanged;
};
