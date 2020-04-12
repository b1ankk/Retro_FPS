#include "AnimatedEntity.h"

#include <SFML/Graphics/RenderTarget.hpp>


#include "Entity.h"
#include "Animation.h"

void game::AnimatedEntity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    // apply the entity's transform -- combine it with the one that was passed by the caller
    states.transform *= sf::Transformable::getTransform();

    states.texture = activeAnimation_->texture().get();

    target.draw(vertices_, states);
}
