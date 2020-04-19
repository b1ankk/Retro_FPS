#pragma once


#include <memory>

#include "vector_additions.h"

namespace game
{
    class LevelMap;
    class Gun;


    class FPP_Player
    {
    public:
        // CONSTRUCTORS

        FPP_Player(const FPP_Player&) = delete;
        ~FPP_Player() = default;
        FPP_Player(const sf::Vector2d& position,
                   const sf::Vector2d& direction,
                   const sf::Vector2d& cameraPlane = sf::Vector2d{0, 0.8888888888888888});

        // CONTROL METHODS

        void rotate(const double&);         // rotate camera in y axis
        void nodHead(const double&);        // simulate camera movement in z/x axis
        void move(const sf::Vector2d&, const double& distance);     // change player's position

        // GAME MECHANICS METHODS

        void shoot() const;
        void takeHit(double damage);

    private:
        // CONTROL

        sf::Vector2d position_{};   // position of the player
        sf::Vector2d direction_{};  // player's facing direction
        sf::Vector2d cameraPlane_{};

        // GAME MECHANICS

        int health_{100};
        int armor_{0};

        std::shared_ptr<Gun> activeGun_;


        void die();

    public:

        // GETTERS AND SETTERS

        [[nodiscard]]
        sf::Vector2d position() const
        {
            return position_;
        }

        [[nodiscard]]
        sf::Vector2d direction() const
        {
            return direction_;
        }

        [[nodiscard]]
        sf::Vector2d cameraPlane() const
        {
            return cameraPlane_;
        }


        [[nodiscard]]
        int health() const 
        {
            return health_;
        }

        [[nodiscard]]
        int armor() const 
        {
            return armor_;
        }


        [[nodiscard]]
        std::shared_ptr<Gun> activeGun() const
        {
            return activeGun_;
        }

        void setActiveGun(std::shared_ptr<Gun> activeGun)
        {
            activeGun_ = std::move(activeGun);
        }
    };
}



