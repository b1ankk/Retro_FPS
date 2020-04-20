#pragma once
#include <queue>
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
        void update() override;
        
    private:
        std::string                                activeAnimationName_;
        std::queue<std::string>                    nextNameQueue_;
        std::unordered_map<std::string, Animation> animations_;



    public:
        void addAnimation(const std::string& name, Animation animation)
        {
            animations_.insert({name, std::move(animation)});
            if (activeAnimationName_.empty())
                activeAnimationName_ = name;
        }

        void setActiveAnimation(const std::string& name)
        {
            if (activeAnimationName_ != name)
            {
                animations_.at(activeAnimationName_).stop();
                activeAnimationName_ = name;
            }
        }

        void queueAnimation(std::string name)
        {
            nextNameQueue_.push(std::move(name));
        }

        [[nodiscard]]
        const std::string& activeAnimationName() const
        {
            return activeAnimationName_;
        }

        [[nodiscard]]
        bool hasAnimationEnded() const
        {
            return animations_.at(activeAnimationName_).hasEnded();
        }

        void playAnimation()
        {
            animations_.at(activeAnimationName_).play();
        }

        void loopAnimation()
        {
            animations_.at(activeAnimationName_).loop();
        }

        void pauseAnimation()
        {
            animations_.at(activeAnimationName_).pause();
        }

        void stopAnimation()
        {
            animations_.at(activeAnimationName_).stopRepeating();
        }

        void forceStopAnimation()
        {
            animations_.at(activeAnimationName_).stop();
        }

    };
}
