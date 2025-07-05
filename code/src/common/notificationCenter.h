#pragma once

#include <functional>
#include <vector>
#include <memory>
#include <unordered_map>
#include <algorithm>

// 通知类型枚举
enum class NotificationType {
    GameReset,
    DamageCollision,
    SlowCollision,
    PowerCollision,
    HealthCollision
};

// 通知数据基类
struct NotificationData {
    NotificationType type;
    virtual ~NotificationData() = default;
};

// 游戏重置通知数据
struct GameResetData : public NotificationData {
    GameResetData() { type = NotificationType::GameReset; }
    bool resetScore = true;
    bool resetLevel = true;
    bool resetPlayerState = true;
};

// 伤害碰撞通知数据
struct DamageCollisionData : public NotificationData {
    DamageCollisionData() { type = NotificationType::DamageCollision; }
};

// 减速碰撞通知数据
struct SlowCollisionData : public NotificationData {
    SlowCollisionData() { type = NotificationType::SlowCollision; }
};

// 加速碰撞通知数据
struct PowerCollisionData : public NotificationData {
    PowerCollisionData() { type = NotificationType::PowerCollision; }
};

// 生命恢复碰撞通知数据
struct HealthCollisionData : public NotificationData {
    HealthCollisionData() { type = NotificationType::HealthCollision; }
};

// 通知观察者接口
class INotificationObserver {
public:
    virtual ~INotificationObserver() = default;
    virtual void onNotification(const NotificationData& data) = 0;
};

// 通知中心 - 单例模式
class NotificationCenter {
public:
    static NotificationCenter& getInstance() {
        static NotificationCenter instance;
        return instance;
    }

    // 订阅通知
    void subscribe(NotificationType type, std::shared_ptr<INotificationObserver> observer) {
        m_observers[type].push_back(observer);
    }

    // 取消订阅
    void unsubscribe(NotificationType type, std::shared_ptr<INotificationObserver> observer) {
        auto& observers = m_observers[type];
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&observer](const std::weak_ptr<INotificationObserver>& weak_obs) {
                    return weak_obs.expired() || weak_obs.lock() == observer;
                }),
            observers.end()
        );
    }

    // 发送通知
    void postNotification(const NotificationData& data) {
        auto& observers = m_observers[data.type];
        
        // 清理过期的观察者
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [](const std::weak_ptr<INotificationObserver>& weak_obs) {
                    return weak_obs.expired();
                }),
            observers.end()
        );

        // 通知所有观察者
        for (auto& weak_observer : observers) {
            if (auto observer = weak_observer.lock()) {
                observer->onNotification(data);
            }
        }
    }

    // 便捷方法：发送游戏重置通知
    void postGameReset(bool resetScore = true, bool resetLevel = true, bool resetPlayerState = true) {
        GameResetData data;
        data.resetScore = resetScore;
        data.resetLevel = resetLevel;
        data.resetPlayerState = resetPlayerState;
        postNotification(data);
    }

private:
    NotificationCenter() = default;
    ~NotificationCenter() = default;
    NotificationCenter(const NotificationCenter&) = delete;
    NotificationCenter& operator=(const NotificationCenter&) = delete;

    std::unordered_map<NotificationType, std::vector<std::weak_ptr<INotificationObserver>>> m_observers;
};
