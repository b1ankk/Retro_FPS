#pragma once
#include <array>
#include <ostream>
#include <SFML/Graphics/RenderWindow.hpp>
#include <string>

#include "FPP_Player.h"
#include "SpriteManager.h"
#include "vector_additions.h"

namespace game
{
    extern std::vector<std::vector<int>> tempWorldMap;

    class Renderer
    {
    public:
        Renderer(const FPP_Player& player,
                 const int&        width,
                 const int&        height,
                 std::shared_ptr
                 <sf::RenderWindow> renderWindow,
                 const SpriteManager& spriteManager,
                 const double&        renderDistance = 32.
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
        std::shared_ptr
        <sf::RenderWindow> const renderWindow_;

        const SpriteManager& spriteManager_;
        sf::Uint8*           screenBuffer_{};

        const FPP_Player&   player_; // player object 
        const sf::Vector2d& plane_;  // camera plane

        double renderDistance_;

        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };

        std::array<std::string, 1> spriteNames
        {
            "wolfensteinBricks"
        };


        // METHODS

        void drawPrimitiveWorld();

        void drawTexturedWorld();
    };
}
