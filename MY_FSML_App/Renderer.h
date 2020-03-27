#pragma once

#include <future>
#include <SFML/Graphics/RenderWindow.hpp>
#include <vector>


#include "FPP_Player.h"

namespace game
{
    class Renderer
    {
        static constexpr int THREADS = 16;


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

        void renderFrame();



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

        std::shared_ptr<const FPP_Player> player_;   // player object 
        const sf::Vector2d&               plane_;    // camera plane
        std::shared_ptr<const LevelMap>   levelMap_; // map of the level

        std::shared_ptr<int> fpsCounter_{nullptr};
        double renderDistance_{32.};

        // FLAGS

        bool drawFpsCounter_{};


        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };
        std::vector<int> renderThreadBounds_;
        // METHODS
        

        void drawPrimitiveWorld();

        void drawTexturedWorld(const int& start, const int& end);

        void drawFPS();


    public:
        bool idDrawfpsCounter() const
        {
            return drawFpsCounter_;
        }

        void setFPSCounter(bool renderFpsCounter)
        {
            drawFpsCounter_ = renderFpsCounter;
        }


        void setFpsPointer(std::shared_ptr<int> fpsCounter)
        {
            fpsCounter_ = std::move(fpsCounter);
        }
    };
}
