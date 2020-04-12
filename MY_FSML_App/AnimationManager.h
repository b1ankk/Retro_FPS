#pragma once
#include <unordered_map>

#include "Animation.h"

namespace game
{
    class Animation;



    class AnimationManager
    {
    public:
        

    private:
        std::unordered_map<std::string, Animation> animations_;

    public:

        void addAnimation(std::string name, Animation animation)
        {
            animations_.insert({std::move(name), std::move(animation)});
        }

        Animation getAnimationForName(const std::string& name)
        {
            return animations_[name];
        }


    };

    
}
