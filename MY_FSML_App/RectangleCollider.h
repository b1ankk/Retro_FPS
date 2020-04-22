#pragma once
#include "Collider.h"

namespace game
{
    class RectangleCollider : public Collider
    {
    public:

        RectangleCollider(sf::Vector2d position, 
                          sf::Vector2d size) :
            Collider(position),
            size_(size)
        {
            
        }


        [[nodiscard]]
        bool isColliding(const RectangleCollider& rect) const override;

        [[nodiscard]] bool
        isColliding(const CircleCollider& circle) const override;

    protected:
        sf::Vector2d size_;

    public:
        [[nodiscard]]
        double left() const 
        {
            return position_.x;
        }

        [[nodiscard]]
        double right() const
        {
            return position_.x + size_.x;
        }

        [[nodiscard]]
        double top() const
        {
            return position_.y;
        }

        [[nodiscard]]
        double bottom() const
        {
            return position_.y + size_.y;
        }

        [[nodiscard]]
        const sf::Vector2d& size() const
        {
            return size_;
        }


    };




    
}
