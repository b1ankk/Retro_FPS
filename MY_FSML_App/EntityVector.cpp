#include "EntityVector.h"

#include "Entity.h"

namespace game
{
    using namespace std;

    void EntityVector::add(shared_ptr<Entity> entity)
    {
        entities_.push_back(move(entity));
    }

    // Removes the element of the specified index. Does NOT preserve the order of elements.
    void EntityVector::remove(const size_t& index)
    {
        if (index != entities_.size() - 1)
            swap(entities_[index], entities_[entities_.size() - 1]);
        entities_.pop_back();
    }


    void EntityVector::updateDistancesTo(const sf::Vector2d& other)
    {
        for (auto& entity : entities_)
        {
            const sf::Vector2d difference = entity->mapPosition_ - other;

            // no need for sqrt since its only for comparison
            entity->distanceToPlayer_     = difference.x * difference.x + difference.y * difference.y;
        }
    }

    void EntityVector::sortEntities()
    {
        sort(
            entities_.begin(),
            entities_.end(),
            [](const shared_ptr<Entity>& left, const shared_ptr<Entity>& right)
            {
                return left->distanceToPlayer() > right->distanceToPlayer();
            }
        );
    }
}
