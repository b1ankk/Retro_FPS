#pragma once

#include <corecrt_math_defines.h>
#include <cmath>
#include <cfloat>

#include "Animation.h"
#include "vector_additions.h"



namespace game
{
    class Enemy;


    class Gun
    {
    public:


        Gun(std::string name,
            Animation shootAnimation,
            const sf::Time shotDelay,
            const double damage,
            const double effectiveAngle = 5.,
            const double range = std::numeric_limits<double>::max())
            : name_(std::move(name)),
              shootAnimation_(std::move(shootAnimation)),
              shotDelay_(shotDelay),
              range_(range),
              damage_(damage),
              effectiveAngle_(effectiveAngle),
              effectiveAngleRad_(effectiveAngle / 180. * M_PI)
              // effectiveAngleVectorRight_(angleToVector(effectiveAngle_))
        {
        }

        void shootIfPossible();


    protected:
        std::string name_{"unnamed"};
        Animation shootAnimation_;

        sf::Time shotDelay_;
        sf::Clock delayCounter_;

        double range_;
        double damage_;
        double effectiveAngle_;
        double effectiveAngleRad_;
        // sf::Vector2d effectiveAngleVectorRight_;
        // sf::Vector2d effectiveAngleVectorLeft_;

        int ammo_{32};
        int maxAmmo_{99};

        // METHODS

        void shoot();
        bool isInAngle(std::shared_ptr<Enemy> enemy);
        double calcDamage(double distanceSquared);


        // static sf::Vector2d angleToVector(double angle)
        // {
        //     angle /= 180.;
        //     angle *= M_PI;
        //
        //     double x = sin(angle);
        //     double y = cos(angle);
        //
        //     return sf::Vector2d{x, y};
        // }


    public:
        // GETTERS & SETTERS


        std::shared_ptr<const sf::Texture> getTexture()
        {
            return shootAnimation_.texture();
        }

        [[nodiscard]]
        sf::Time shotDelay() const
        {
            return shotDelay_;
        }

        [[nodiscard]]
        double range() const
        {
            return range_;
        }

        [[nodiscard]]
        double damage() const
        {
            return damage_;
        }

        [[nodiscard]]
        double effectiveAngle() const
        {
            return effectiveAngle_;
        }

        void addAmmo(int amount)
        {
            ammo_ += amount;
            if (ammo_ > maxAmmo_)
                ammo_ = maxAmmo_;
        }

        [[nodiscard]]
        int ammo() const
        {
            return ammo_;
        }

        [[nodiscard]]
        int maxAmmo() const
        {
            return maxAmmo_;
        }
    };
}

