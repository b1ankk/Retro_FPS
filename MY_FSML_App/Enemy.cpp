#include "Enemy.h"

#include <cassert>
#include <random>

#include "Game.h"
#include "GameTime.h"
#include "Gun.h"
#include "LevelMap.h"

namespace game
{
    void Enemy::takeHit(double damage)
    {
        if (isAlive_ && damage > 0)
        {
            bleed();
            health_ -= static_cast<int>(damage);
            if (health_ <= 0)
                die();
        }

    }

    void Enemy::walk(const sf::Vector2d& rawDirection, const double distance)
    {
        assert(!walkAnimationName_.empty());

        const sf::Vector2d startPosition{mapPosition()};

        const sf::Vector2d difference = normalizeVector2(rawDirection) * distance;
        const sf::Vector2d newPosition{mapPosition() + difference};

        collider().setPosition(newPosition);
        sf::Vector2i newGridPos{static_cast<sf::Vector2i>(newPosition)};
        auto& map = *Game::get().levelMap()->mapData();

        sf::Vector2i next{
            newGridPos.x + static_cast<int>(copysign(1, rawDirection.x)),
            newGridPos.y + static_cast<int>(copysign(1, rawDirection.y))
        };

        // if (!isColliding(map[next.x][int(mapPosition().x)])  &&  
        //     !isColliding(map[int(mapPosition().x)][next.y])  &&
        //     !isColliding(map[next.x][next.y]))
        // {
        //     setMapPosition(newPosition);
        //
        //     queueAnimation(walkAnimationName_);
        // }
        // else
        // {
        //     collider().setPosition(mapPosition());
        // }

        collider().setPosition(newPosition.x, mapPosition().y);
        if (!isColliding(map[next.x][static_cast<int>(mapPosition().y)]))
        {
            if (Game::get().levelMap()->isInBounds(next))
            {
                if (!isColliding(map[next.x][next.y]))
                    setMapPosition({newPosition.x, mapPosition().y});
                else
                {
                    while (isColliding(map[next.x][next.y]))
                        collider().setPosition(collider().position() - rawDirection * 0.005);

                    setMapPosition(collider().position());
                }
            }
            else
            {
                setMapPosition({newPosition.x, mapPosition().y});
            }
        }

        collider().setPosition(mapPosition().x, newPosition.y);
        if (!isColliding(map[static_cast<int>(mapPosition().x)][next.y]))
        {
            if (Game::get().levelMap()->isInBounds(next))
            {
                if (!isColliding(map[next.x][next.y]))
                    setMapPosition({mapPosition().x, newPosition.y});
                else
                {
                    while (isColliding(map[next.x][next.y]))
                        collider().setPosition(collider().position() - rawDirection * 0.005);

                    setMapPosition(collider().position());
                }
            }
            else
            {
                setMapPosition({mapPosition().x, newPosition.y});
            }
        }

        collider().setPosition(mapPosition());

        if (mapPosition() != startPosition)
            queueAnimation(walkAnimationName_);


    }

    void Enemy::attack(GameObjRef<FPP_Player> player)
    {
        player->takeHit(attackDamage_);
    }

    void Enemy::die()
    {
        assert(!dieAnimationName_.empty());
        isAlive_ = false;
        queueAnimation(dieAnimationName_);
        playAnimation();

        // TODO better ammo system
        static std::uniform_int_distribution<int> dist(0, 4);
        Game::get().player()->activeGun()->addAmmo(dist(Game::get().randEngine()));
    }


    void Enemy::update()
    {
        AnimatedEntity::update();

        if (isAlive())
        {
            if (squaredDistanceToPlayer() != 0)
            {
                if (squaredDistanceToPlayer() <= 0.81)
                {
                    Game::get().player()->takeHit(attackDamage_);
                }

                if (squaredDistanceToPlayer() <= 64 || health_ < maxHealth_)
                {
                    followPlayer_ = true;
                }

                if (followPlayer_ && squaredDistanceToPlayer() > 0.16)
                {
                    sf::Vector2d playerDirection{Game::get().player()->position() - mapPosition()};

                    double distance = movementSpeed_ * GameTime::deltaTime() / 1000.;

                    walk(playerDirection, distance);
                }
            }
        }

    }

    void Enemy::bleed()
    {
        queueAnimation(bleedAnimationName_);
        queueAnimation(idleAnimationName_);
        while (activeAnimationName() == walkAnimationName_)
            forceNextAnimation();
    }
}
