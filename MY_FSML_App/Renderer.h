#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "FPP_Player.h"

namespace game
{
    extern game::LevelMap tempWorldMap;

    class Renderer
    {
    public:
        Renderer(std::shared_ptr<const FPP_Player> player,
                 const int&                        width,
                 const int&                        height,
                 std::shared_ptr<sf::RenderWindow> renderWindow,
                 const SpriteManager&              spriteManager,
                 const double&                     renderDistance = 32.
        );

        ~Renderer();

        void renderFrame()
        {
            renderWindow_->clear(sf::Color::Black);
            drawPrimitiveWorld();
            renderWindow_->display();
        }

    private:
        // FIELDS

        int width_;  // width to render
        int height_; // height to render
        std::shared_ptr<sf::RenderWindow> renderWindow_;
        // sf::RenderWindow&        renderWindow_;

        const SpriteManager& spriteManager_;
        sf::Uint8*           screenBuffer_{};

        std::shared_ptr<const FPP_Player> player_; // player object 
        const sf::Vector2d&               plane_;  // camera plane

        double renderDistance_;

        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };



        // METHODS

        void drawPrimitiveWorld();

        void drawTexturedWorld();
    };
}
