#include "Gun.h"

namespace game
{
    void Gun::shootIfPossible()
    {
        if (delayCounter_.getElapsedTime() > shotDelay_)
        {
            if (ammo_ > 0)
            {
                shoot();
                --ammo_;
            }
        }
        

    }


    void Gun::shoot()
    {
        delayCounter_.restart();
        shootAnimation_.stop();
        shootAnimation_.play();

        // TODO damage mechanics
    }
}
