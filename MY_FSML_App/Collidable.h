#pragma once
#include <memory>

#include "Collider.h"


namespace game
{

    class Collidable
    {
    public:
        Collidable();
        Collidable(std::shared_ptr<Collider> collider);
        Collidable(const Collidable& collidable);


        // TODO COLLIDABLE COPYING CONSTRUCTOR 

        [[nodiscard]]
        virtual bool isColliding(const Collidable& collidable) const;

    private:
        std::shared_ptr<Collider> collider_;

    protected:

        Collider& collider()
        {
            return *collider_;
        }

    public:
        void setCollider(std::shared_ptr<Collider> collider)
        {
            collider_ = std::move(collider);
        }

        [[nodiscard]]
        bool hasCollider() const
        {
            return collider_ != nullptr;
        }

    };
   
}
