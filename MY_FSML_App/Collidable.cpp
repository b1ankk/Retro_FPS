#include "Collidable.h"

#include <utility>


#include "RectangleCollider.h"
#include "CircleCollider.h"

namespace game
{
    using namespace std;

    Collidable::Collidable(shared_ptr<Collider> collider):
        collider_(std::move(collider))
    {
    }

    bool Collidable::isColliding(const Collidable& collidable) const
    {
        Collider* collider = collidable.collider_.get();

        if (collider == nullptr || this->collider_ == nullptr)
            return false;

        if (dynamic_cast<const RectangleCollider*>(collider) != nullptr)
            return this->collider_->isColliding(*dynamic_cast<const RectangleCollider*>(collider));

        if (dynamic_cast<const CircleCollider*>(collider) != nullptr)
            return this->collider_->isColliding(*dynamic_cast<const CircleCollider*>(collider));


        return this->collider_->isColliding(*collider);
    }
    
}
