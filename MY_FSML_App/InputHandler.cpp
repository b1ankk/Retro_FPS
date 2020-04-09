#include <SFML/Graphics.hpp>

#include "InputHandler.h"

#include "FPP_Player.h"
#include "GameTime.h"


namespace game
{
    InputHandler::InputHandler()/* :
        window_(GAME::WINDOW),
        player_(GAME::PLAYER)*/
    {
        setup();
    }

    void InputHandler::setup()
    {
        Game::get().window()->setMouseCursorVisible(false);
        sf::Mouse::setPosition(Game::SCREEN_MIDDLE, *Game::get().window());
    }


    void InputHandler::handleInput()
    {
        if (!Game::get().window()->hasFocus())
            return;

        handleMouseLook();
        handleKeyMovement();
    }


    void InputHandler::handleMouseLook()
    {
        // X-AXIS LOOK
        int offsetX = sf::Mouse::getPosition(*Game::get().window()).x - Game::SCREEN_MIDDLE.x;
        double degrees = static_cast<double>(offsetX) / Game::get().window()->getSize().x * mouseLookSensitivity_.x;
        Game::get().player()->rotate(degrees);
 
        // TODO Y-AXIS LOOK


        sf::Mouse::setPosition(Game::SCREEN_MIDDLE, *Game::get().window());
    }

    void InputHandler::handleKeyMovement()
    {
        sf::Vector2d direction{};

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
            direction += Game::get().player()->direction();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
            direction -= Game::get().player()->direction();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            double rotatedDirectionX = Game::get().player()->direction().y;
            double rotatedDirectionY = -Game::get().player()->direction().x;

            direction.x += rotatedDirectionX;
            direction.y += rotatedDirectionY;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            double rotatedDirectionX = Game::get().player()->direction().y;
            double rotatedDirectionY = -Game::get().player()->direction().x;

            direction.x -= rotatedDirectionX;
            direction.y -= rotatedDirectionY;
        }


        // TODO a more proper implementation of walk and run
        // sprint and walk
        double multiplier{1.};
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift))
            multiplier = 2.;
        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
            multiplier = 0.25;

        if (direction.x != 0 || direction.y != 0)
            Game::get().player()->move(direction, movementSpeed_ / 1000. * multiplier * GameTime::deltaTime());
    }
}

