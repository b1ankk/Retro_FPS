#include "CircleCollider.h"

#include <cmath>


#include "RectangleCollider.h"

namespace game
{
    bool CircleCollider::isColliding(const RectangleCollider& rect) const
    {
        return rect.isColliding(*this);
    }


    bool CircleCollider::isColliding(const CircleCollider& circle) const
    {
        auto posDiff = position_ - circle.position_;
        double distance = sqrt(posDiff.x * posDiff.x + posDiff.y * posDiff.y);

        return distance <= radius_ + circle.radius_;
    }
}
