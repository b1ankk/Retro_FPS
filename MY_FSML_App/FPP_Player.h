#pragma once
#include <iostream>
#include <ostream>
#include <SFML/Graphics.hpp>

#include "vector_additions.h"



namespace game
{
    extern std::vector<std::vector<int>> tempWorldMap;

    class FPP_Player
    {
    public:
        FPP_Player(const sf::Vector2d& position,
                   const sf::Vector2d& direction,
                   const sf::Vector2d& cameraPlane = sf::Vector2d{0, 0.66});
            

        void rotate(const double&);         // rotate camera in y axis
        void nodHead(const double&);        // simulate camera movement in z/x axis
        void move(const sf::Vector2d&, const double& distance);     // change player's position

        // GETTERS AND SETTERS

        const sf::Vector2d& position() const
        {
            return position_;
        }

        const sf::Vector2d& direction() const
        {
            return direction_;
        }
        const sf::Vector2d& cameraPlane() const
        {
            return cameraPlane_;
        }

    private:
        sf::Vector2d position_{};   // position of the player
        sf::Vector2d direction_{};  // player's facing direction
        sf::Vector2d cameraPlane_{};

    };
}


