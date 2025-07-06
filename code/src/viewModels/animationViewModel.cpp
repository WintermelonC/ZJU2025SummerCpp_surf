#include "animationViewModel.h"

// SpriteAnimationInstance 实现
SpriteAnimationInstance::SpriteAnimationInstance()
    : m_state(AnimationState::Stopped),
      m_currentFrameIndex(0) {
}

void SpriteAnimationInstance::play(const std::string& animationName, 
                                 const std::map<std::string, AnimationConfig>& animations) {
    auto it = animations.find(animationName);
    if (it == animations.end()) {
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

void SpriteAnimationInstance::pause() {
    if (m_state == AnimationState::Playing) {
        m_state = AnimationState::Paused;
    }
}

void SpriteAnimationInstance::resume() {
    if (m_state == AnimationState::Paused) {
        m_state = AnimationState::Playing;
        m_frameClock.restart();
    }
}

void SpriteAnimationInstance::stop() {
    m_state = AnimationState::Stopped;
    reset();
}

void SpriteAnimationInstance::update(float deltaTime, 
                                   const std::map<std::string, AnimationConfig>& animations) {
    if (m_state != AnimationState::Playing || m_currentAnimationName.empty()) {
        return;
    }
    
    auto it = animations.find(m_currentAnimationName);
    if (it == animations.end()) {
        return;
    }
    
    const auto& currentAnim = it->second;
    
    // 检查是否需要切换到下一帧
    if (m_frameClock.getElapsedTime().asSeconds() >= currentAnim.frameInterval) {
        nextFrame(animations);
        m_frameClock.restart();
    }
}

TextureType SpriteAnimationInstance::getCurrentFrame(const std::map<std::string, AnimationConfig>& animations) const {
    if (m_currentAnimationName.empty()) {
        return TextureType::player_center_1; // 默认纹理
    }
    
    auto it = animations.find(m_currentAnimationName);
    if (it == animations.end()) {
        return TextureType::player_center_1;
    }
    
    const auto& currentAnim = it->second;
    if (m_currentFrameIndex >= 0 && m_currentFrameIndex < static_cast<int>(currentAnim.frames.size())) {
        return currentAnim.frames[m_currentFrameIndex];
    }
    
    return TextureType::player_center_1; // 默认纹理
}

void SpriteAnimationInstance::reset() {
    m_currentFrameIndex = 0;
    m_frameClock.restart();
}

void SpriteAnimationInstance::nextFrame(const std::map<std::string, AnimationConfig>& animations) {
    if (m_currentAnimationName.empty()) {
        return;
    }
    
    auto it = animations.find(m_currentAnimationName);
    if (it == animations.end()) {
        return;
    }
    
    const auto& currentAnim = it->second;
    
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
                m_onAnimationComplete(m_spriteId, m_currentAnimationName);
            }
        }
    }
    
    // 触发帧变化事件
    if (m_onFrameChanged && oldFrameIndex != m_currentFrameIndex) {
        m_onFrameChanged(m_spriteId, m_currentFrameIndex, getCurrentFrame(animations));
    }
}

// AnimationViewModel 实现
AnimationViewModel::AnimationViewModel() {
    // 注册玩家精灵到动画管理器
    registerSprite("player");
    initialize();
}

void AnimationViewModel::subscribeToNotifications() {
    //  订阅游戏重置通知
    auto& notificationCenter = NotificationCenter::getInstance();
    notificationCenter.subscribe(NotificationType::GameReset, shared_from_this());
}

void AnimationViewModel::initialize() {
    // 初始化动画
    addAnimation("center", AnimationConfig({
        TextureType::player_center_1,
        TextureType::player_center_2,
        TextureType::player_center_3
    }, 0.1f, true));

    addAnimation("left1", AnimationConfig({
        TextureType::player_left_11,
        TextureType::player_left_12,
        TextureType::player_left_13
    }, 0.1f, true));

    addAnimation("left2", AnimationConfig({
        TextureType::player_left_21,
        TextureType::player_left_22,
        TextureType::player_left_23
    }, 0.1f, true));

    addAnimation("right1", AnimationConfig({
        TextureType::player_right_11,
        TextureType::player_right_12,
        TextureType::player_right_13
    }, 0.1f, true));

    addAnimation("right2", AnimationConfig({
        TextureType::player_right_21,
        TextureType::player_right_22,
        TextureType::player_right_23
    }, 0.1f, true));

    addAnimation("stop", AnimationConfig({
        TextureType::player_stop_1,
        TextureType::player_stop_2,
        TextureType::player_stop_3
    }, 0.1f, true));

    addAnimation("waiting", AnimationConfig({
        TextureType::player_wait_1,
        TextureType::player_wait_2,
        TextureType::player_wait_3
    }, 0.1f, true));

    playAnimation("player", "center");
}

void AnimationViewModel::reset() {
    // 重置动画到初始状态
    playAnimation("player", "center");
}

void AnimationViewModel::update(const float deltaTime) {
    // 更新所有精灵的动画
    updateAllAnimations(deltaTime);
    
    // 更新 player 的纹理
    updatePlayerAnimation();
}

void AnimationViewModel::updatePlayerAnimation() {
    // 更新玩家动画
    std::string targetAnimation;
    
    switch (*m_playerState) {
        case Config::PlayerState::center:
            targetAnimation = "center";
            break;
        case Config::PlayerState::left1:
            targetAnimation = "left1";
            break;
        case Config::PlayerState::left2:
            targetAnimation = "left2";
            break;
        case Config::PlayerState::right1:
            targetAnimation = "right1";
            break;
        case Config::PlayerState::right2:
            targetAnimation = "right2";
            break;
        case Config::PlayerState::stop:
            targetAnimation = "stop";
            break;
    }

    if (m_isPlayerWaiting && *m_isPlayerWaiting) {
        // 如果玩家处于等待状态，使用等待动画
        targetAnimation = "waiting";
    }
    
    // 如果需要切换动画，则切换
    if (getCurrentAnimationName("player") != targetAnimation) {
        playAnimation("player", targetAnimation);
    }

    m_playerTexture = getCurrentFrame("player");
}

void AnimationViewModel::addAnimation(const std::string& name, const AnimationConfig& config) {
    m_animations[name] = config;
}

bool AnimationViewModel::hasAnimation(const std::string& name) const {
    return m_animations.find(name) != m_animations.end();
}

void AnimationViewModel::registerSprite(const std::string& spriteId) {
    if (m_spriteInstances.find(spriteId) == m_spriteInstances.end()) {
        auto instance = std::make_unique<SpriteAnimationInstance>();
        instance->setSpriteId(spriteId);
        
        // 设置全局回调
        if (m_globalOnAnimationComplete) {
            instance->setOnAnimationComplete(m_globalOnAnimationComplete);
        }
        if (m_globalOnFrameChanged) {
            instance->setOnFrameChanged(m_globalOnFrameChanged);
        }
        
        m_spriteInstances[spriteId] = std::move(instance);
    }
}

void AnimationViewModel::unregisterSprite(const std::string& spriteId) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        m_spriteInstances.erase(it);
    }
}

bool AnimationViewModel::hasSpriteRegistered(const std::string& spriteId) const {
    return m_spriteInstances.find(spriteId) != m_spriteInstances.end();
}

void AnimationViewModel::playAnimation(const std::string& spriteId, const std::string& animationName) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end() && hasAnimation(animationName)) {
        it->second->play(animationName, m_animations);
    }
}

void AnimationViewModel::pauseAnimation(const std::string& spriteId) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        it->second->pause();
    }
}

void AnimationViewModel::resumeAnimation(const std::string& spriteId) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        it->second->resume();
    }
}

void AnimationViewModel::stopAnimation(const std::string& spriteId) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        it->second->stop();
    }
}

void AnimationViewModel::playAnimationForAll(const std::string& animationName) {
    if (!hasAnimation(animationName)) {
        return;
    }
    
    for (auto& pair : m_spriteInstances) {
        pair.second->play(animationName, m_animations);
    }
}

void AnimationViewModel::pauseAllAnimations() {
    for (auto& pair : m_spriteInstances) {
        pair.second->pause();
    }
}

void AnimationViewModel::resumeAllAnimations() {
    for (auto& pair : m_spriteInstances) {
        pair.second->resume();
    }
}

void AnimationViewModel::stopAllAnimations() {
    for (auto& pair : m_spriteInstances) {
        pair.second->stop();
    }
}

void AnimationViewModel::updateAllAnimations(float deltaTime) {
    for (auto& pair : m_spriteInstances) {
        pair.second->update(deltaTime, m_animations);
    }

    // 更新 player 的 texture
    m_playerTexture = getCurrentFrame("player");
}

TextureType AnimationViewModel::getCurrentFrame(const std::string& spriteId) const {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        return it->second->getCurrentFrame(m_animations);
    }
    return TextureType::player_center_1; // 默认纹理
}

bool AnimationViewModel::isPlaying(const std::string& spriteId) const {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        return it->second->isPlaying();
    }
    return false;
}

bool AnimationViewModel::isPaused(const std::string& spriteId) const {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        return it->second->isPaused();
    }
    return false;
}

bool AnimationViewModel::isStopped(const std::string& spriteId) const {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        return it->second->isStopped();
    }
    return true;
}

std::string AnimationViewModel::getCurrentAnimationName(const std::string& spriteId) const {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        return it->second->getCurrentAnimationName();
    }
    return "";
}

std::vector<std::string> AnimationViewModel::getRegisteredSpriteIds() const {
    std::vector<std::string> ids;
    ids.reserve(m_spriteInstances.size());
    
    for (const auto& pair : m_spriteInstances) {
        ids.push_back(pair.first);
    }
    
    return ids;
}

void AnimationViewModel::setOnAnimationComplete(std::function<void(const std::string&, const std::string&)> callback) {
    m_globalOnAnimationComplete = callback;
    
    // 为所有现有实例设置回调
    for (auto& pair : m_spriteInstances) {
        pair.second->setOnAnimationComplete(callback);
    }
}

void AnimationViewModel::setOnFrameChanged(std::function<void(const std::string&, int, TextureType)> callback) {
    m_globalOnFrameChanged = callback;
    
    // 为所有现有实例设置回调
    for (auto& pair : m_spriteInstances) {
        pair.second->setOnFrameChanged(callback);
    }
}

void AnimationViewModel::setOnAnimationCompleteForSprite(const std::string& spriteId, 
                                                       std::function<void(const std::string&, const std::string&)> callback) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        it->second->setOnAnimationComplete(callback);
    }
}

void AnimationViewModel::setOnFrameChangedForSprite(const std::string& spriteId, 
                                                  std::function<void(const std::string&, int, TextureType)> callback) {
    auto it = m_spriteInstances.find(spriteId);
    if (it != m_spriteInstances.end()) {
        it->second->setOnFrameChanged(callback);
    }
}

void AnimationViewModel::onNotification(const NotificationData& data) {
    switch (data.type) {
        case NotificationType::GameReset: {
            const auto& resetData = static_cast<const GameResetData&>(data);
            if (resetData.resetPlayerState) {
                reset();
            }
            break;
        }
        default:
            break;
    }
}