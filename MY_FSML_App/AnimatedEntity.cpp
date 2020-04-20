#include "AnimatedEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>


#include "Entity.h"
#include "Animation.h"

void game::AnimatedEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= sf::Transformable::getTransform();

    Animation* activeAnimation = const_cast<Animation*>(&animations_.at(activeAnimationName_));

    states.texture = activeAnimation->texture().get();

    target.draw(vertices_, states);
}

void game::AnimatedEntity::update()
{
    Entity::update();

    auto& activeAnimation = animations_.at(activeAnimationName_);
    if (!nextNameQueue_.empty() && (activeAnimation.hasEnded() || activeAnimation.isRepeating()))
    {
        activeAnimation.pause();
        activeAnimationName_ = std::move(nextNameQueue_.front());
        nextNameQueue_.pop();

        animations_.at(activeAnimationName_).stop();
        animations_.at(activeAnimationName_).play();
    }

}


