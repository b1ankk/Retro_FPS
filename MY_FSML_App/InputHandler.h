#pragma once

#include "GameObjRef.h"
#include "vector_additions.h"
#include "Game.h"





namespace game
{
    class FPP_Player;
    


    class InputHandler
    {
    public:
        InputHandler(const InputHandler&) = delete;
        InputHandler();
        ~InputHandler() = default;

        void handleInput();

    private:

        // FIELDS

        // bool cursorLocked_{true};

        sf::Vector2d mouseLookSensitivity_{1., 1.};
        double       movementSpeed_{1.};

        // METHODS
        void setup();

        void handleMouseLook();
        void handleKeyMovement();

    public:
        // SETTERS & GETTERS

        [[nodiscard]]
        auto mouseLookSensitivity() const
        {
            return mouseLookSensitivity_;
        }

        [[nodiscard]]
        auto movementSpeed() const
        {
            return movementSpeed_;
        }

        void setMouseLookSensitivityX(const double& mouseLookSensitivityX)
        {
            mouseLookSensitivity_.x = mouseLookSensitivityX;
        }

        void setMouseLookSensitivityY(const double& mouseLookSensitivityY)
        {
            mouseLookSensitivity_.y = mouseLookSensitivityY;
        }

        void setMouseLookSensitivity(const sf::Vector2d& mouseLookSensitivity)
        {
            mouseLookSensitivity_ = mouseLookSensitivity;
        }

        void setMovementSpeed(const double& movementSpeed)
        {
            movementSpeed_ = movementSpeed;
        }
    };
}
