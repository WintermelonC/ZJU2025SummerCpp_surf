#include "../animationViewModel.h"

Config::AnimationUpdateCallback AnimationViewModel::getAnimationCommands() {
    return [this](const float& deltaTime) {
        update(deltaTime);
    };
}