#include "models/entityModel.h"
#include <cmath>

EntityModel::EntityModel(EntityType type)
    : m_position(0.0f, 0.0f),
      m_velocity(0.0f, 0.0f),
      m_angle(sf::degrees(0.0f)),
      m_size(1.0f, 1.0f),
      m_type(type),
      m_state(EntityState::Active) {
}

sf::FloatRect EntityModel::getBounds() const {
    return sf::FloatRect({m_position.x - m_size.x / 2.0f, m_position.y - m_size.y / 2.0f}, m_size);
}

sf::FloatRect EntityModel::getCollisionBounds() const {
    // 默认情况下，碰撞边界与实体边界相同
    // 子类可以重写此方法来提供更精确的碰撞检测
    return getBounds();
}

bool EntityModel::intersects(const EntityModel& other) const {
    if (!isActive() || !other.isActive()) {
        return false;
    }
    
    sf::FloatRect thisBounds = getCollisionBounds();
    sf::FloatRect otherBounds = other.getCollisionBounds();
    
    // 手动实现碰撞检测，使用正确的成员名称
    return thisBounds.findIntersection(otherBounds).has_value();
}

void EntityModel::update(float deltaTime) {
    if (!isActive()) {
        return;
    }
    
    // 基础位置更新
    sf::Vector2f oldPosition = m_position;
    m_position += m_velocity * deltaTime;
    
    // 如果位置发生变化，触发事件
    if (oldPosition != m_position) {
        notifyPositionChanged();
    }
}
