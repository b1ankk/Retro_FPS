#include "FPP_Player.h"

#include <corecrt_math_defines.h>


#include "LevelMap.h"
#include "MapTile.h"
#include "Gun.h"


namespace game
{
    FPP_Player::FPP_Player(const sf::Vector2d& position,
                           const sf::Vector2d& direction,
                           const sf::Vector2d& cameraPlane) :
        position_(position),
        direction_(direction),
        cameraPlane_(cameraPlane)
    {
    }

    void FPP_Player::rotate(const double& degrees)
    {
        const double angle{degrees * M_PI / 180.};

        // both camera direction and camera plane must be rotated
        const double oldDirectionX = direction_.x;
        direction_.x               = direction_.x * cos(-angle) - direction_.y * sin(-angle);
        direction_.y               = oldDirectionX * sin(-angle) + direction_.y * cos(-angle);
        const double oldPlaneX     = cameraPlane_.x;
        cameraPlane_.x             = cameraPlane_.x * cos(-angle) - cameraPlane_.y * sin(-angle);
        cameraPlane_.y             = oldPlaneX * sin(-angle) + cameraPlane_.y * cos(-angle);
    }

    void FPP_Player::nodHead(const double&)
    {
        // TODO define FPP_Player::nodHead(const double&)
    }


    void FPP_Player::move(const sf::Vector2d& rawDirection, const double& distance)
    {
        const sf::Vector2d difference = normalizeVector2(rawDirection) * distance;
        const sf::Vector2d newPosition{position_ + difference};

        // TODO better collision system


        if ((*Game::get().levelMap()->mapData())
            [static_cast<size_t>(newPosition.x)][static_cast<int>(position_.y)].isTraversable())
        {
            position_.x = newPosition.x;
        }

        if ((*Game::get().levelMap()->mapData())
            [static_cast<int>(position_.x)][static_cast<int>(newPosition.y)].isTraversable())
        {
            position_.y = newPosition.y;
        }


        // if (tempWorldMap[static_cast<int>(newPosition.x)][static_cast<int>(position_.y)] == 0)
        //     position_.x = newPosition.x;
        // if (tempWorldMap[static_cast<int>(position_.x)][static_cast<int>(newPosition.y)] == 0)
        //     position_.y = newPosition.y;
    }

    void FPP_Player::shoot() const
    {
        assert(activeGun_ != nullptr);

        activeGun_->shootIfPossible();
    }

    void FPP_Player::takeHit(double damage)
    {
        health_ -= static_cast<int>(damage);
        if (health_ <= 0)
            die();
    }

    void FPP_Player::die()
    {
        // TODO dying
    }
}
