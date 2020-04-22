#include "FPP_Player.h"


#include <cassert>
#include <corecrt_math_defines.h>

#include "Game.h"
#include "LevelMap.h"
#include "MapTile.h"
#include "Gun.h"
#include "CircleCollider.h"


namespace game
{
    FPP_Player::FPP_Player(const sf::Vector2d& position,
                           const sf::Vector2d& direction,
                           const sf::Vector2d& cameraPlane) :
        Collidable(std::make_shared<CircleCollider>(position, 0.25)),
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

        sf::Vector2i newGridPos{static_cast<sf::Vector2i>(newPosition)};
        auto& map = *Game::get().levelMap()->mapData();

        collider().setPosition(newPosition.x, position_.y);
        if (!isColliding(map[newGridPos.x][static_cast<int>(position_.y)]) &&
            !isColliding(map[newGridPos.x + static_cast<size_t>(copysign(1, rawDirection.x))][static_cast<int>(position_.y)]))
            position_.x = newPosition.x;
        else
            position_.x = newGridPos.x + (rawDirection.x < 0 ? 0.25 : 0.75);

        collider().setPosition(position_.x, newPosition.y);
        if (!isColliding(map[static_cast<int>(position_.x)][newGridPos.y]) && 
            !isColliding(map[static_cast<int>(position_.x)][newGridPos.y + static_cast<size_t>(copysign(1, rawDirection.y))]))
            position_.y = newPosition.y;
        else if (map[static_cast<int>(position_.x)][newGridPos.y].isTraversable())
            position_.y = newGridPos.y + (rawDirection.y < 0 ? 0.25 : 0.75);

        collider().setPosition(position_);
       
    }

    void FPP_Player::shoot() const
    {
        assert(activeGun_ != nullptr);

        activeGun_->shootIfPossible();
    }

    void FPP_Player::takeHit(double damage)
    {
        if (hitDelayClock_.getElapsedTime() > hitDelayTime_)
        {
            health_ -= static_cast<int>(damage);
            if (health_ <= 0)
                die();

            hitDelayClock_.restart();
        }
    }

    void FPP_Player::die()
    {
        // TODO dying
    }
}
