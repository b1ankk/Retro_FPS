#include "Gun.h"

#include <iostream>
#include <ostream>

#include "Game.h"
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

    double Gun::calcDamage(double distanceSquared)
    {
        static const double divider = log(25.);
        double dmg = log(-sqrt(distanceSquared) + 25.) / divider;
        dmg *= damage_;
        std::cout << dmg << std::endl;
        if (dmg > 0)
            return dmg;
        return 0;
    }

    void Gun::shoot()
    {
        delayCounter_.restart();
        shootAnimation_.stop();
        shootAnimation_.play();

        auto distances = Game::get().renderer()->perpWallDistances();
        const int wallIndex = static_cast<int>(distances.size()) / 2;
        const double wallDistance = distances[wallIndex];

        for (auto& enemy : Game::get().levelMap()->enemies())
        {
            if (sqrt(enemy->squaredDistanceToPlayer()) < wallDistance  &&  isInAngle(enemy))
            {
                std::cout << "HIT!" << std::endl;
                enemy->takeHit(calcDamage(enemy->squaredDistanceToPlayer()));
            }

        }
    }
}
