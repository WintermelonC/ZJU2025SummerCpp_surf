#pragma once

#include <SFML/Graphics.hpp>
#include <functional>
#include "common/config.h"

// 实体类型枚举
enum class EntityType {
    Player,
    Obstacle,
    PowerUp,
    Other
};

// 实体状态枚举
enum class EntityState {
    Active,
    Inactive,
    Destroyed
};

// 基础实体模型
class EntityModel {
public:
    EntityModel(EntityType type = EntityType::Other);
    virtual ~EntityModel() = default;
    
    // 基础属性获取器
    const sf::Vector2f& getPosition() const { return m_position; }
    const sf::Vector2f& getVelocity() const { return m_velocity; }
    sf::Angle getAngle() const { return m_angle; }
    const sf::Vector2f& getSize() const { return m_size; }
    EntityType getType() const { return m_type; }
    EntityState getState() const { return m_state; }
    bool isActive() const { return m_state == EntityState::Active; }
    
    // 基础属性设置器
    void setPosition(const sf::Vector2f& position) { m_position = position; }
    void setVelocity(const sf::Vector2f& velocity) { m_velocity = velocity; }
    void setAngle(sf::Angle angle) { m_angle = angle; }
    void setSize(const sf::Vector2f& size) { m_size = size; }
    void setState(EntityState state) { m_state = state; }
    
    // 碰撞检测相关
    sf::FloatRect getBounds() const;
    virtual sf::FloatRect getCollisionBounds() const;
    bool intersects(const EntityModel& other) const;
    
    // 基础更新逻辑
    virtual void update(float deltaTime);
    
    // 状态管理
    virtual void activate() { m_state = EntityState::Active; }
    virtual void deactivate() { m_state = EntityState::Inactive; }
    virtual void destroy() { m_state = EntityState::Destroyed; }
    
    // 事件回调
    void setOnStateChanged(std::function<void(EntityState)> callback) { 
        m_onStateChanged = callback; 
    }
    void setOnPositionChanged(std::function<void(const sf::Vector2f&)> callback) { 
        m_onPositionChanged = callback; 
    }

protected:
    // 触发状态变化事件
    void notifyStateChanged() {
        if (m_onStateChanged) {
            m_onStateChanged(m_state);
        }
    }
    
    // 触发位置变化事件
    void notifyPositionChanged() {
        if (m_onPositionChanged) {
            m_onPositionChanged(m_position);
        }
    }

protected:
    // 基础属性
    sf::Vector2f m_position;
    sf::Vector2f m_velocity;
    sf::Angle m_angle;
    sf::Vector2f m_size;
    EntityType m_type;
    EntityState m_state;
    
    // 事件回调
    std::function<void(EntityState)> m_onStateChanged;
    std::function<void(const sf::Vector2f&)> m_onPositionChanged;
};
