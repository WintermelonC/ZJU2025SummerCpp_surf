#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include <string>
#include <functional>
#include "common/config.h"
#include "common/assetManager.h"

// 动画状态
enum class AnimationState {
    Playing,
    Paused,
    Stopped
};

// 动画配置
struct AnimationConfig {
    std::vector<Textures> frames;    // 动画帧
    float frameInterval;             // 帧间隔时间（秒）
    bool loop;                       // 是否循环播放
    
    // 默认构造函数
    AnimationConfig() : frameInterval(0.1f), loop(true) {}
    
    AnimationConfig(const std::vector<Textures>& animFrames, 
                   float interval = 0.1f, 
                   bool isLoop = true)
        : frames(animFrames), frameInterval(interval), loop(isLoop) {}
};

// 动画控制器
class AnimationController {
public:
    AnimationController();
    
    // 添加动画
    void addAnimation(const std::string& name, const AnimationConfig& config);
    
    // 播放控制
    void play(const std::string& animationName);
    void pause();
    void resume();
    void stop();
    
    // 更新动画
    void update(float deltaTime);
    
    // 获取当前帧
    Textures getCurrentFrame() const;
    
    // 状态查询
    bool isPlaying() const { return m_state == AnimationState::Playing; }
    bool isPaused() const { return m_state == AnimationState::Paused; }
    bool isStopped() const { return m_state == AnimationState::Stopped; }
    bool hasAnimation(const std::string& name) const;
    const std::string& getCurrentAnimationName() const { return m_currentAnimationName; }
    
    // 动画事件回调
    void setOnAnimationComplete(std::function<void(const std::string&)> callback) {
        m_onAnimationComplete = callback;
    }
    void setOnFrameChanged(std::function<void(int, Textures)> callback) {
        m_onFrameChanged = callback;
    }
    
private:
    // 重置动画状态
    void reset();
    
    // 切换到下一帧
    void nextFrame();

private:
    std::map<std::string, AnimationConfig> m_animations;
    std::string m_currentAnimationName;
    AnimationState m_state;
    
    sf::Clock m_frameClock;
    int m_currentFrameIndex;
    
    // 事件回调
    std::function<void(const std::string&)> m_onAnimationComplete;
    std::function<void(int, Textures)> m_onFrameChanged;
};
