#include "Collidable.h"

#include <utility>


#include "RectangleCollider.h"
#include "CircleCollider.h"

namespace game
{
    using namespace std;

    Collidable::Collidable() :
        Collidable(nullptr)
    {
        
    }

    Collidable::Collidable(shared_ptr<Collider> collider):
        collider_(std::move(collider))
    {

    }


    Collidable::Collidable(const Collidable& collidable)
    {
        if (collidable.hasCollider())
        {
            Collider* col = collidable.collider_.get();

            if (dynamic_cast<RectangleCollider*>(col) != nullptr)
                collider_ = make_shared<RectangleCollider>(*reinterpret_cast<RectangleCollider*>(col));

            else if (dynamic_cast<CircleCollider*>(col))
                collider_ = make_shared<CircleCollider>(*reinterpret_cast<CircleCollider*>(col));

            else
                collider_ = make_shared<Collider>(*collidable.collider_);

        }
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
