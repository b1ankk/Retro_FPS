#pragma once
#include <memory>

#include "Collider.h"


namespace game
{

    class Collidable
    {
    public:
        Collidable(std::shared_ptr<Collider> collider);


    private:
        std::shared_ptr<Collider> collider_;

    protected:

        Collider& collider()
        {
            return *collider_;
        }

    public:


        [[nodiscard]]
        virtual bool isColliding(const Collidable& collidable) const;
    };
   
}
