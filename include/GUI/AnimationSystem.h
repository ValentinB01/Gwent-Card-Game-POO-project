#include <SFML/Graphics.hpp>
#include <functional>

class AnimationSystem {
    public:
        struct Animation {
            sf::Vector2f startPos;
            sf::Vector2f endPos;
            float duration;
            float elapsed;
            std::function<void(float)> updateCallback;
        };
    
        void addAnimation(Animation anim);
        void update(float deltaTime);
        
    private:
        std::vector<Animation> activeAnimations;
    };
    