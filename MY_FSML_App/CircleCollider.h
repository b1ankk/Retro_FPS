#pragma once
#include "Collider.h"

namespace game
{
    class CircleCollider : public Collider
    {
    public:


        CircleCollider(const sf::Vector2d& position,
                       double radius) :
            Collider(position),
            radius_(radius)
        {
        }

        [[nodiscard]]
        bool isColliding(const RectangleCollider& rect) const override;

        [[nodiscard]]
        bool isColliding(const CircleCollider& circle) const override;

    protected:
        double radius_;


    public:
        [[nodiscard]]
        double radius() const
        {
            return radius_;
        }



    };
}
