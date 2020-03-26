#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

#include "FPP_Player.h"

namespace game
{
    

    class Renderer
    {
    public:
        Renderer(std::shared_ptr<const FPP_Player>    player,
                 const int&                           width,
                 const int&                           height,
                 std::shared_ptr<sf::RenderWindow>    renderWindow,
                 std::shared_ptr<const SpriteManager> spriteManager,
                 std::shared_ptr<const LevelMap>      levelMap,
                 const double&                        renderDistance = 32.
        );

        ~Renderer();

        void renderFrame()
        {
            renderWindow_->clear(sf::Color{0xff});
            drawTexturedWorld();
            renderWindow_->display();
        }

    private:
        // FIELDS

        int                               width_;  // width to render
        int                               height_; // height to render
        std::shared_ptr<sf::RenderWindow> renderWindow_;
        // sf::RenderWindow&        renderWindow_;

        std::shared_ptr<const SpriteManager> spriteManager_;
        size_t                               screenBufferLength_{};
        sf::Uint8*                           screenBuffer_{};
        sf::Uint8*                           screenClearBuffer_{nullptr};

        std::shared_ptr<const FPP_Player> player_; // player object 
        const sf::Vector2d&               plane_;  // camera plane
        std::shared_ptr<const LevelMap>   levelMap_;

        double renderDistance_;

        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };


        // METHODS

        void drawPrimitiveWorld();

        void drawTexturedWorld();
    };
}
