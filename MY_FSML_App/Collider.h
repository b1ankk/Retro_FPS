#pragma once
#include "vector_additions.h"

namespace game
{
    class RectangleCollider;
    class CircleCollider;

    class Collider
    {
    public:


        [[nodiscard]]
        virtual bool isColliding(const Collider& collider) const;

        [[nodiscard]]
        virtual bool isColliding(const RectangleCollider& rect) const;

        [[nodiscard]]
        virtual bool isColliding(const CircleCollider& circle) const;

    protected:
        explicit Collider(const sf::Vector2d& position)
            : position_(position)
        {

        }

        sf::Vector2d position_;


    public:
    
        // GETTERS & SETTERS
    
        [[nodiscard]]
        const sf::Vector2d& position() const
        {
            return position_;
        }
    
        void setPosition(const sf::Vector2d& position)
        {
            position_ = position;
        }
    
        void setPosition(double x, double y)
        {
            position_ = {x, y};
        }

    };
}
