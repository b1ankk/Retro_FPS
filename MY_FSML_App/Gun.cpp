#include "Gun.h"

#include <iostream>
#include <ostream>



#include "Enemy.h"
#include "LevelMap.h"

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


    bool Gun::isInAngle(std::shared_ptr<Enemy> enemy)
    {
        const sf::Vector2d& playerDir = Game::get().player()->direction();
        const sf::Vector2d relativePos = enemy->mapPosition() - Game::get().player()->position();
        double cosValue = calcDotProduct(relativePos, playerDir) / (calcVectorLength(playerDir)* calcVectorLength(relativePos));
        double angle = acos(cosValue);
        
        return abs(angle) < effectiveAngleRad_;
    }

    void Gun::shoot()
    {
        delayCounter_.restart();
        shootAnimation_.stop();
        shootAnimation_.play();

        for (auto& enemy : Game::get().levelMap()->enemies())
        {
            if (isInAngle(enemy))
            {
                std::cout << "HIT!" << std::endl;
                enemy->takeHit(damage_);
            }

        }
    }
}
