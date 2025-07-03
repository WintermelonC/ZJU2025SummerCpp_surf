#include "animationViewModel.h"

AnimationViewModel::AnimationViewModel()
    : m_state(AnimationState::Stopped),
      m_currentFrameIndex(0) {
}

void AnimationViewModel::addAnimation(const std::string& name, const AnimationConfig& config) {
    m_animations[name] = config;
}

void AnimationViewModel::play(const std::string& animationName) {
    if (!hasAnimation(animationName)) {
        return;
    }
    
    // 如果切换到不同的动画，重置状态
    if (m_currentAnimationName != animationName) {
        m_currentAnimationName = animationName;
        reset();
    }
    
    m_state = AnimationState::Playing;
    m_frameClock.restart();
}

void AnimationViewModel::pause() {
    if (m_state == AnimationState::Playing) {
        m_state = AnimationState::Paused;
    }
}

void AnimationViewModel::resume() {
    if (m_state == AnimationState::Paused) {
        m_state = AnimationState::Playing;
        m_frameClock.restart();
    }
}

void AnimationViewModel::stop() {
    m_state = AnimationState::Stopped;
    reset();
}

void AnimationViewModel::update(float deltaTime) {
    if (m_state != AnimationState::Playing || m_currentAnimationName.empty()) {
        return;
    }
    
    const auto& currentAnim = m_animations[m_currentAnimationName];
    
    // 检查是否需要切换到下一帧
    if (m_frameClock.getElapsedTime().asSeconds() >= currentAnim.frameInterval) {
        nextFrame();
        m_frameClock.restart();
    }
}

TextureType AnimationViewModel::getCurrentFrame() const {
    if (m_currentAnimationName.empty()) {
        return TextureType::player_center_1; // 默认纹理
    }
    
    const auto& currentAnim = m_animations.at(m_currentAnimationName);
    if (m_currentFrameIndex >= 0 && m_currentFrameIndex < static_cast<int>(currentAnim.frames.size())) {
        return currentAnim.frames[m_currentFrameIndex];
    }
    
    return TextureType::player_center_1; // 默认纹理
}

bool AnimationViewModel::hasAnimation(const std::string& name) const {
    return m_animations.find(name) != m_animations.end();
}

void AnimationViewModel::reset() {
    m_currentFrameIndex = 0;
    m_frameClock.restart();
}

void AnimationViewModel::nextFrame() {
    if (m_currentAnimationName.empty()) {
        return;
    }
    
    const auto& currentAnim = m_animations[m_currentAnimationName];
    
    if (currentAnim.frames.empty()) {
        return;
    }
    
    int oldFrameIndex = m_currentFrameIndex;
    m_currentFrameIndex++;
    
    // 检查是否到达动画末尾
    if (m_currentFrameIndex >= static_cast<int>(currentAnim.frames.size())) {
        if (currentAnim.loop) {
            m_currentFrameIndex = 0; // 循环播放
        } else {
            m_currentFrameIndex = static_cast<int>(currentAnim.frames.size()) - 1; // 停在最后一帧
            m_state = AnimationState::Stopped;
            
            // 触发动画完成事件
            if (m_onAnimationComplete) {
                m_onAnimationComplete(m_currentAnimationName);
            }
        }
    }
    
    // 触发帧变化事件
    if (m_onFrameChanged && oldFrameIndex != m_currentFrameIndex) {
        m_onFrameChanged(m_currentFrameIndex, getCurrentFrame());
    }
}
