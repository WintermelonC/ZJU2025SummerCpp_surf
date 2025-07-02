#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "common/config.h"
#include "common/animationController.h"
#include "models/entityModel.h"

enum class PlayerState {
    Left2, Left1, Center, Right1, Right2, Stop
};

// 玩家数据模型
class PlayerModel : public EntityModel {
public:
    PlayerModel();
    
    // 继承自EntityModel的方法无需重新声明
    // getPosition(), getVelocity(), getAngle(), setPosition(), setVelocity(), setAngle() 等
    
    // 玩家特有的获取器
    PlayerState getPlayerState() const { return m_playerState; }
    int getHP() const { return m_hp; }
    int getPower() const { return m_power; }
    bool isTurn() const { return m_isTurn; }
    bool isPower() const { return m_isPower; }
    
    // 动画相关
    const AnimationController& getAnimationController() const { return m_animationController; }
    AnimationController& getAnimationController() { return m_animationController; }
    Textures getCurrentAnimationFrame() const;
    
    // 玩家特有的设置器
    void setPlayerState(PlayerState state) { m_playerState = state; }
    void setTurn(bool isTurn) { m_isTurn = isTurn; }
    void setPower(bool isPower) { m_isPower = isPower; }
    
    // 重写基类方法
    void update(float deltaTime) override;
    sf::FloatRect getCollisionBounds() const override;
    
    // 玩家特有的业务逻辑
    void usePower();
    void takeDamage();
    void resetPlayer();
    
    // 玩家特有的事件回调
    void setOnHPChanged(std::function<void(int)> callback) { 
        m_onHPChanged = callback; 
    }
    void setOnPowerChanged(std::function<void(int)> callback) { 
        m_onPowerChanged = callback; 
    }

private:
    // 动画相关私有方法
    void initializeAnimations();
    void updateAnimation();

private:
    // 玩家特有属性
    PlayerState m_playerState;
    int m_hp;
    int m_power;
    bool m_isTurn;
    bool m_isPower;
    
    // 动画控制器
    AnimationController m_animationController;
    
    // 玩家特有的事件回调
    std::function<void(int)> m_onHPChanged;
    std::function<void(int)> m_onPowerChanged;
};
