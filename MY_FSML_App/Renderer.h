#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Color.hpp>
#include <chrono>

#include "GameObjRef.h"
#include "UserInterface.h"

namespace game
{
    using namespace std::chrono;

    class FPP_Player;
    class SpriteManager;
    class LevelMap;
    class Renderer;


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
        void drawGameOverScreen();



        class FilterColor
        {
        public:
            FilterColor() :
                FilterColor(sf::Color{0x0}, 0ms)
            {
            }

            FilterColor(const FilterColor& other)
                : color_(other.color_),
                  stepDuration(other.stepDuration),
                  timePoint_(steady_clock::now())
            {
            }

            FilterColor& operator=(const FilterColor& other)
            {
                if (this == &other)
                    return *this;

                color_       = other.color_;
                stepDuration = other.stepDuration;
                timePoint_   = steady_clock::now();
                return *this;
            }

            FilterColor(const sf::Color& color, milliseconds fadeDuration) :
                color_(color),
                stepDuration(color.a != 0 ? fadeDuration / color.a : 0ms),
                timePoint_(steady_clock::now())
            {
            }

            sf::Color color()
            {
                if (color_.a == 0 || stepDuration == 0ms)
                    return sf::Color::Transparent;

                while (color_.a > 0 && steady_clock::now() - timePoint_ > stepDuration)
                {
                    timePoint_ += stepDuration;
                    --color_.a;
                }

                // printf("color: %x, stepDuration: %lld, time: %lld, now - time: %lld\n",
                //        color_.toInteger(), stepDuration.count(), timePoint_.time_since_epoch().count(), (steady_clock::now() - timePoint_).count()
                // );

                return color_;
            }


        private:
            sf::Color                color_;
            milliseconds             stepDuration;
            time_point<steady_clock> timePoint_;
        };

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

        sf::RectangleShape screenFilter_;
        FilterColor        filterColor_;

        // SWITCHES

        bool drawFpsCounter_{};


        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };

        std::vector<int> renderThreadBounds_;

        // METHODS

        void initVertices();

        void drawPrimitiveWorld();

        void drawTexturedWorld(const int& start, const int& end);

        void drawEntities();

        void drawFPS();

        std::pair<double, double> findTransitionPoint(double left, double right, const double perpDist);


    public:

        GameObjRef<UserInterface> userInterface() const
        {
            return userInterface_;
        }

        const std::vector<double>& perpWallDistances() const
        {
            return perpWallDistances_;
        }

        bool ifDrawFpsCounter() const
        {
            return drawFpsCounter_;
        }

        void setDrawFpsCounter(bool renderFpsCounter)
        {
            drawFpsCounter_ = renderFpsCounter;
        }

        void applyColorFilter(FilterColor colorFilter)
        {
            filterColor_ = colorFilter;
        }
    };
}
