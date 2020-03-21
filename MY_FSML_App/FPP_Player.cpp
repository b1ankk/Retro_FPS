#define _USE_MATH_DEFINES
#include <cmath>
#include "FPP_Player.h"
#include "vector_additions.h"


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
        double oldDirectionX = direction_.x;
        direction_.x         = direction_.x * cos(-angle) - direction_.y * sin(-angle);
        direction_.y         = oldDirectionX * sin(-angle) + direction_.y * cos(-angle);
        double oldPlaneX     = cameraPlane_.x;
        cameraPlane_.x       = cameraPlane_.x * cos(-angle) - cameraPlane_.y * sin(-angle);
        cameraPlane_.y       = oldPlaneX * sin(-angle) + cameraPlane_.y * cos(-angle);
    }

    void FPP_Player::nodHead(const double&)
    {
        // TODO define FPP_Player::nodHead(const double&)
    }


    void FPP_Player::move(const sf::Vector2d& rawDirection, const double& distance)
    {
        sf::Vector2d differece = normalizeVector2(rawDirection) * distance;
        sf::Vector2d newPosition{position_ + differece};

        // TODO better collision system


        if (tempWorldMap[static_cast<int>(newPosition.x)][static_cast<int>(position_.y)] == 0)
            position_.x = newPosition.x;
        if (tempWorldMap[static_cast<int>(position_.x)][static_cast<int>(newPosition.y)] == 0)
            position_.y = newPosition.y;
    }
}
