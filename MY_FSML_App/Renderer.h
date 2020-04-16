#pragma once

#include <SFML/Graphics.hpp>

#include "UserInterface.h"

namespace game
{
    class FPP_Player;
    class SpriteManager;
    class LevelMap;


    class Renderer
    {
        static constexpr int THREADS = 16;


    public:
        Renderer(const Renderer&) = delete;

        Renderer(const int&    width,
                 const int&    height,
                 const double& renderDistance = 32.
        );

        ~Renderer();

        void renderFrame();

        void initUI();


    private:
        // FIELDS

        int width_{};  // width to render
        int height_{}; // height to render

        // Screen buffer operation fields

        size_t     screenBufferLength_{};
        sf::Uint8* screenBuffer_{};
        sf::Uint8* screenClearBuffer_{nullptr};


        double renderDistance_{32.};

        // Rendering objects

        sf::Image        frame_{};
        sf::VertexArray  vertices_{};
        sf::Texture      texture_{};
        sf::RenderStates renderStates_{};

        std::shared_ptr<UserInterface> userInterface_;

        std::vector<double> perpWallDistances_;

        // SWITCHES

        bool drawFpsCounter_{};


        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };

        std::vector<int> renderThreadBounds_;

        // METHODS

        void drawPrimitiveWorld();

        void drawTexturedWorld(const int& start, const int& end);

        void drawEntities();

        void drawFPS();

        void initVertices();


        std::pair<double, double> findTransitionPoint(double left, double right, const double perpDist);


    public:
        bool ifDrawFpsCounter() const
        {
            return drawFpsCounter_;
        }

        void setDrawFpsCounter(bool renderFpsCounter)
        {
            drawFpsCounter_ = renderFpsCounter;
        }
    };
}
