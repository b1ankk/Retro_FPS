#include "Collider.h"
#include "RectangleCollider.h"
#include "CircleCollider.h"

namespace game
{
    bool Collider::isColliding(const Collider& collider) const 
    {
        return false;
    }

    bool Collider::isColliding(const RectangleCollider& rect) const
    {
        return Collider::isColliding(static_cast<const Collider&>(rect));
    }

    bool Collider::isColliding(const CircleCollider& circle) const 
    {
        return Collider::isColliding(static_cast<const Collider&>(circle));
    }

    
    
}
