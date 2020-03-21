#include "InputHandler.h"

namespace game
{
    InputHandler::InputHandler(sf::RenderWindow& window,
                               FPP_Player& player) :
        window_(window),
        player_(player)
    {
        setup();
    }

    void InputHandler::setup()
    {
        window_.setMouseCursorVisible(false);
        sf::Mouse::setPosition(screenMiddle_, window_);
    }


    void InputHandler::handleInput()
    {
        handleMouseLook();
        handleKeyMovement();
    }


    void InputHandler::handleMouseLook()
    {
        // X-AXIS LOOK
        int offsetX = sf::Mouse::getPosition(window_).x - screenMiddle_.x;
        double degrees = static_cast<double>(offsetX) / window_.getSize().x * mouseLookSensitivity_.x;
        player_.rotate(degrees);
 
        // TODO Y-AXIS LOOK


        sf::Mouse::setPosition(screenMiddle_, window_);
    }

    void InputHandler::handleKeyMovement()
    {
        sf::Vector2d direction{};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            direction += player_.direction();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            direction -= player_.direction();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            double rotatedDirectionX = player_.direction().y;
            double rotatedDirectionY = -player_.direction().x;

            direction.x += rotatedDirectionX;
            direction.y += rotatedDirectionY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            double rotatedDirectionX = player_.direction().y;
            double rotatedDirectionY = -player_.direction().x;

            direction.x -= rotatedDirectionX;
            direction.y -= rotatedDirectionY;
        }

        if (direction.x != 0 || direction.y != 0)
            player_.move(direction, movementSpeed_ * 0.01);
    }
}

