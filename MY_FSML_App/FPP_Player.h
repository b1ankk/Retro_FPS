#pragma once

#include "Game.h"
#include "vector_additions.h"

namespace game
{
    class LevelMap;


    class FPP_Player
    {
    public:
        FPP_Player(const FPP_Player&) = delete;
        ~FPP_Player() = default;
        FPP_Player(const sf::Vector2d& position,
                   const sf::Vector2d& direction,
                   const sf::Vector2d& cameraPlane = sf::Vector2d{0, 0.8888888888888888});
            

        void rotate(const double&);         // rotate camera in y axis
        void nodHead(const double&);        // simulate camera movement in z/x axis
        void move(const sf::Vector2d&, const double& distance);     // change player's position

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

    private:
        sf::Vector2d position_{};   // position of the player
        sf::Vector2d direction_{};  // player's facing direction
        sf::Vector2d cameraPlane_{};
    };
}



