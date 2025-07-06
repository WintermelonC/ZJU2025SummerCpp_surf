#pragma once

#include <vector>
#include <map>
#include <string>
#include <memory>
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "../common/config.h"
#include "../common/notificationCenter.h"
#include "textureViewModel.h"

// 动画状态
enum class AnimationState {
    Playing,
    Paused,
    Stopped
};

// 动画配置
struct AnimationConfig {
    std::vector<TextureType> frames;    // 动画帧
    float frameInterval;                // 帧间隔时间（秒）
    bool loop;                         // 是否循环播放
    
    // 默认构造函数
    AnimationConfig() : frameInterval(0.1f), loop(true) {}
    
    AnimationConfig(const std::vector<TextureType>& animFrames, 
                    float interval = 0.1f, 
                    bool isLoop = true)
        : frames(animFrames), frameInterval(interval), loop(isLoop) {}
};

// 精灵动画实例
class SpriteAnimationInstance {
public:
    SpriteAnimationInstance();
    
    // 播放控制
    void play(const std::string& animationName, const std::map<std::string, AnimationConfig>& animations);
    void pause();
    void resume();
    void stop();
    
    // 更新动画
    void update(float deltaTime, const std::map<std::string, AnimationConfig>& animations);
    
    // 获取当前帧
    TextureType getCurrentFrame(const std::map<std::string, AnimationConfig>& animations) const;
    
    // 状态查询
    bool isPlaying() const { return m_state == AnimationState::Playing; }
    bool isPaused() const { return m_state == AnimationState::Paused; }
    bool isStopped() const { return m_state == AnimationState::Stopped; }
    const std::string& getCurrentAnimationName() const { return m_currentAnimationName; }
    
    // 动画事件回调设置
    void setOnAnimationComplete(std::function<void(const std::string&, const std::string&)> callback) {
        m_onAnimationComplete = callback; // 第一个参数是spriteId，第二个是动画名
    }
    void setOnFrameChanged(std::function<void(const std::string&, int, TextureType)> callback) {
        m_onFrameChanged = callback; // 第一个参数是spriteId
    }
    
    void setSpriteId(const std::string& spriteId) { m_spriteId = spriteId; }
    
private:
    void reset();
    void nextFrame(const std::map<std::string, AnimationConfig>& animations);

private:
    std::string m_spriteId;
    std::string m_currentAnimationName;
    AnimationState m_state;
    sf::Clock m_frameClock;
    int m_currentFrameIndex;
    
    // 事件回调
    std::function<void(const std::string&, const std::string&)> m_onAnimationComplete;
    std::function<void(const std::string&, int, TextureType)> m_onFrameChanged;
};

// 多精灵动画管理器
class AnimationViewModel : public INotificationObserver, public std::enable_shared_from_this<AnimationViewModel> {
public:
    AnimationViewModel();

    // 初始化和重置
    void initialize();
    void reset();
    void update(const float deltaTime);

    // 在初始化完成后调用，用于订阅通知
    void subscribeToNotifications();
    
    // 实现观察者接口
    void onNotification(const NotificationData& data) override;
    
    // 动画配置管理
    void addAnimation(const std::string& name, const AnimationConfig& config);
    bool hasAnimation(const std::string& name) const;
    
    // 精灵实例管理
    void registerSprite(const std::string& spriteId);
    void unregisterSprite(const std::string& spriteId);
    bool hasSpriteRegistered(const std::string& spriteId) const;
    
    // 播放控制 - 针对特定精灵
    void playAnimation(const std::string& spriteId, const std::string& animationName);
    void pauseAnimation(const std::string& spriteId);
    void resumeAnimation(const std::string& spriteId);
    void stopAnimation(const std::string& spriteId);
    
    // 播放控制 - 针对所有精灵
    void playAnimationForAll(const std::string& animationName);
    void pauseAllAnimations();
    void resumeAllAnimations();
    void stopAllAnimations();
    
    // 更新所有精灵动画
    void updateAllAnimations(float deltaTime);
    
    // 获取特定精灵的当前帧
    TextureType getCurrentFrame(const std::string& spriteId) const;
    const TextureType& getPlayerTexture() const { return m_playerTexture; }
    
    // 状态查询
    bool isPlaying(const std::string& spriteId) const;
    bool isPaused(const std::string& spriteId) const;
    bool isStopped(const std::string& spriteId) const;
    std::string getCurrentAnimationName(const std::string& spriteId) const;
    
    // 获取所有注册的精灵ID
    std::vector<std::string> getRegisteredSpriteIds() const;
    Config::AnimationUpdateCallback getAnimationCommands();
    
    // 全局事件回调设置
    void setOnAnimationComplete(std::function<void(const std::string&, const std::string&)> callback);
    void setOnFrameChanged(std::function<void(const std::string&, int, TextureType)> callback);
    void setPlayerState(const Config::PlayerState* playerState) { m_playerState = playerState; }
    
    // 为特定精灵设置事件回调
    void setOnAnimationCompleteForSprite(const std::string& spriteId, 
                                       std::function<void(const std::string&, const std::string&)> callback);
    void setOnFrameChangedForSprite(const std::string& spriteId, 
                                  std::function<void(const std::string&, int, TextureType)> callback);

private:
    void updatePlayerAnimation();

private:
    // 全局动画配置
    std::map<std::string, AnimationConfig> m_animations;
    
    // 精灵实例管理
    std::unordered_map<std::string, std::unique_ptr<SpriteAnimationInstance>> m_spriteInstances;
    
    // 全局事件回调
    std::function<void(const std::string&, const std::string&)> m_globalOnAnimationComplete;
    std::function<void(const std::string&, int, TextureType)> m_globalOnFrameChanged;

    // 玩家动画相关
    TextureType m_playerTexture;
    const Config::PlayerState* m_playerState; // 用于获取玩家状态的配置
};
