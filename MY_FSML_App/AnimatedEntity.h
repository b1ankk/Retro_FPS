#pragma once
#include <unordered_map>
#include <xstring>

#include "Animation.h"
#include "Entity.h"

namespace game
{
    class Animation;


    class AnimatedEntity : public Entity
    {
    public:
        AnimatedEntity(const std::shared_ptr<const sf::Texture>& texture,
                       const sf::Vector2d&                       mapPosition,
                       const sf::Vector2i&                       imageSize = {64, 64}) // TODO change the constructor
            : Entity(texture, mapPosition, imageSize)
        {
        }

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        
    private:
        Animation*                                 activeAnimation_{};
        std::unordered_map<std::string, Animation> animations_;

    public:
        void addAnimation(const std::string& name, Animation animation)
        {
            animations_.insert({name, std::move(animation)});
            if (activeAnimation_ == nullptr)
                activeAnimation_ = &animations_[name];
        }

        void setActiveAnimation(const std::string& name)
        {
            activeAnimation_->stop();
            activeAnimation_ = &animations_[name];
        }

        void playAnimation()
        {
            activeAnimation_->play();
        }

        void loopAnimation()
        {
            activeAnimation_->loop();
        }

        void pauseAnimation()
        {
            activeAnimation_->pause();
        }

        void stopAnimation()
        {
            activeAnimation_->stopRepeating();
        }

        void forceStopAnimation()
        {
            activeAnimation_->stop();
        }

    };
}
