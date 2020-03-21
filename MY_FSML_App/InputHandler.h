#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "FPP_Player.h"

namespace game
{
    class InputHandler
    {
    public:
        InputHandler(sf::RenderWindow& window, FPP_Player& player);

        void handleInput();

    private:
        // REFERENCES
        sf::RenderWindow& window_;
        FPP_Player& player_;

        // FIELDS

        // bool cursorLocked_{true};

        sf::Vector2d mouseLookSensitivity_{1., 1.};
        double movementSpeed_{1.};

        sf::Vector2i screenMiddle_{
            static_cast<int>(window_.getSize().x) / 2,
            static_cast<int>(window_.getSize().y) / 2
        };

        
        // METHODS
        void setup();

        void handleMouseLook();
        void handleKeyMovement();

    public:
        // SETTERS & GETTERS

        void setMouseLookSensitivityX(const double mouseLookSensitivityX)
        {
            mouseLookSensitivity_.x = mouseLookSensitivityX;
        }

        void setMouseLookSensitivityY(const double mouseLookSensitivityY)
        {
            mouseLookSensitivity_.y = mouseLookSensitivityY;
        }

        void setMouseLookSensitivity(const sf::Vector2d& mouseLookSensitivity)
        {
            mouseLookSensitivity_ = mouseLookSensitivity;
        }

        void setMovementSpeed(double movementSpeed)
        {
            movementSpeed_ = movementSpeed;
        }
    };
}

