#include "RectangleCollider.h"

#include <cmath>
#include <iostream>


#include "CircleCollider.h"

namespace game
{
    bool RectangleCollider::isColliding(const RectangleCollider& rect) const
    {
        return left() < rect.right() &&
                right() > rect.left() &&
                top() < rect.bottom() &&
                bottom() > rect.top();
    }

    bool RectangleCollider::isColliding(const CircleCollider& circle) const
    {
        double testX = circle.position().x;
        double testY = circle.position().y;

        if (circle.position().x < left())
            testX = left();
        else if (circle.position().x > right())
            testX = right();

        if (circle.position().y < top())
            testY = top();
        else if (circle.position().y > bottom())
            testY = bottom();


        double distX = circle.position().x - testX;
        double distY = circle.position().y - testY;

        double distance = sqrt(distX * distX + distY * distY);

        return distance <= circle.radius();
    }
}
