#pragma once
#include <memory>

#include "AnimatedEntity.h"
#include "FPP_Player.h"
#include "GameObjRef.h"

namespace game
{
    class Enemy : public AnimatedEntity
    {
    public:
        Enemy(const std::shared_ptr<const sf::Texture>& texture,
              const sf::Vector2d& mapPosition,
              const int                                 maxHealth,
              const double                              attackDamage,
              const double                              movementSpeed = 1.0,
              const double sightRadius = 8.)
            : AnimatedEntity(texture, mapPosition),
              maxHealth_(maxHealth),
              health_(maxHealth_),
              attackDamage_(attackDamage),
              movementSpeed_(movementSpeed),
              sightRadius_(sightRadius),
              sightRadiusSquared_(sightRadius_ * sightRadius_)
        {
        }

        virtual void takeHit(double damage);
        virtual void walk(const sf::Vector2d& direction, const double distance);
        virtual void attack(GameObjRef<FPP_Player>);

    protected:
        const int maxHealth_;
        int       health_;
        double    attackDamage_;
        double    movementSpeed_;
        double sightRadius_;
        double sightRadiusSquared_;

        bool isBleeding{false};
        bool isAlive_{true};
        bool followPlayer_{false};

        std::string idleAnimationName_;
        std::string walkAnimationName_;
        std::string dieAnimationName_;
        std::string bleedAnimationName_;


        virtual void die();
        virtual void bleed();
        virtual void update() override;

    public:

        // GETTERS & SETTERS

        int maxHealth() const
        {
            return maxHealth_;
        }

        int health() const
        {
            return health_;
        }

        double attackDamage() const
        {
            return attackDamage_;
        }

        bool isAlive() const
        {
            return isAlive_;
        }

        void setIdleAnimationName(std::string idleAnimationName)
        {
            idleAnimationName_ = std::move(idleAnimationName);
        }

        void setWalkAnimationName(std::string walkAnimationName)
        {
            walkAnimationName_ = std::move(walkAnimationName);
        }

        void setDieAnimationName(std::string dieAnimationName)
        {
            dieAnimationName_ = std::move(dieAnimationName);
        }

        void setBleedAnimationName(std::string bleedAnimationName)
        {
            bleedAnimationName_ = std::move(bleedAnimationName);
        }
    };
}
