#include "Enemy.h"

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
        assert(dieAnimationName_ != "");
        isAlive_ = false;
        setActiveAnimation(dieAnimationName_);
        playAnimation();
    }
}
