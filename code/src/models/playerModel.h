#pragma once

#include <cmath>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "entityModel.h"

class PlayerModel : public EntityModel {
public:
    PlayerModel();
    ~PlayerModel() = default;

    // 主要更新方法
    void update(const float deltaTime, const sf::Vector2f& mousePos);
    void usePower();
    void reset();

    // Getter 方法
    const sf::Vector2f& getVelocity() const { return m_velocity; }
    const sf::Angle& getAngle() const { return m_angle; }
    const Config::PlayerState& getState() const { return m_state; }
    const int& getHp() const { return m_hp; }
    const int& getPower() const { return m_power; }
    const bool& isPower() const { return m_isPower; }
    const bool& isTurn() const { return m_isTurn; }
    const bool& isWaiting() const { return m_isWaiting; }

    // Setter 方法
    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setAngle(const sf::Angle& angle) { m_angle = angle; }
    void setWaiting(const bool& isWaiting) { m_isWaiting = isWaiting; }

    // 碰撞效果处理方法
    void takeDamage(int damage);
    void applySlowEffect();
    void restoreHealth(int healthAmount);
    void restorePower(int powerAmount);
    
    // 状态查询方法
    const bool& isInvincible() const { return m_isInvincible; }
    void setInvincible();
    bool isSlowed() const { return m_isSlowed; }

private:
    // 内部更新方法
    void updateState(const sf::Vector2f& mousePos);
    void updateTurn();
    void updateAngle();
    void updateYSpeed(const float deltaTime);
    void updateXSpeed(const float deltaTime);
    void updatePower(const float& dt);
    void updateSlow(const float& dt);
    void updateInvincible(const float& dt);

private:
    // 物理常量
    const float m_acceleration1 = 10.f;
    const float m_acceleration2 = 50.f;
    const float m_maxSpeed = 50.f;
    const float m_XYScale1 = 0.4f;
    const float m_XYScale2 = 0.8f;
    const float m_powerScale = 1.5f;
    const float m_angle1 = 20.f;
    const float m_angle2 = 40.f;
    const float m_powerTime = 5.f;

    // 状态变量
    sf::Vector2f m_velocity = {0, 0};
    sf::Angle m_angle = sf::degrees(0.0f);
    Config::PlayerState m_state = Config::PlayerState::center;
    Config::PlayerState m_lastState;  // 上一个状态
    bool m_isTurn = false;  // 是否转弯
    bool m_isPower = false;
    float m_powerTimer = 0.0f;  // 能量计时器
    int m_hp = Config::Player::PLAYER_HP;  // 玩家生命值
    int m_power = 0;  // 玩家能量值
    
    // 状态效果相关
    bool m_isSlowed = false;
    float m_slowFactor = 0.5f;
    float m_slowTimer = 0.0f;
    const float m_slowDuration = 0.1f;
    
    // 无敌时间相关
    bool m_isInvincible = false;
    float m_invincibleTimer = 0.0f;
    const float m_invincibleDuration = 3.0f;  // 默认无敌时间5秒

    // 等待状态
    bool m_isWaiting = false;
};