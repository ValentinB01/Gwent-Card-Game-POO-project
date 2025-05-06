#include "../include/GUI/AnimationSystem.h"

void AnimationSystem::update(float deltaTime) {
    for (auto it = activeAnimations.begin(); it != activeAnimations.end(); ) {
        it->elapsed += deltaTime;
        float progress = std::min(1.0f, it->elapsed / it->duration);
        
        it->updateCallback(progress);
        
        if (progress >= 1.0f) {
            it = activeAnimations.erase(it);
        } else {
            ++it;
        }
    }
}
