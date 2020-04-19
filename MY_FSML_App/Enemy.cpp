#include "Enemy.h"

#include <cassert>
#include <random>

#include "Game.h"
#include "Gun.h"

namespace game
{
    void Enemy::takeHit(double damage)
    {
        if (isAlive_)
        {
            health_ -= static_cast<int>(damage);
            if (health_ <= 0)
                die();
        }

    }

    void Enemy::walk(const sf::Vector2d& direction, const double distance)
    {
        auto normDir = normalizeVector2(direction);
        assert(walkAnimationName_ != "");
        setActiveAnimation(walkAnimationName_);
        playAnimation();
        setPosition(getPosition() + static_cast<sf::Vector2f>(normDir * distance));
    }

    void Enemy::attack(GameObjRef<FPP_Player> player)
    {
        player->takeHit(attackDamage_);
    }

    void Enemy::die()
    {
        assert(!dieAnimationName_.empty());
        isAlive_ = false;
        setActiveAnimation(dieAnimationName_);
        playAnimation();


        // TODO better ammo system
        std::uniform_int_distribution<int> dist(0, 4);
        Game::get().player()->activeGun()->addAmmo(dist(Game::get().randEngine()));
    }


}
