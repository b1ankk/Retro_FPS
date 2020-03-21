#pragma once
#include <ostream>

#include "FPP_Player.h"
#include "vector_additions.h"

namespace game
{
    extern sf::RenderWindow*             g_windowPtr;
    extern std::vector<std::vector<int>> tempWorldMap;

    class Renderer
    {
    public:
        Renderer(const FPP_Player& player,
                 const int&        width,
                 const int&        height,
                 const double&     renderDistance = 32.
        );

        void renderFrame();

    private:
        int width_;  // width to render
        int height_; // height to render

        const FPP_Player&   player_; // player object 
        const sf::Vector2d& plane_;  // camera plane

        double renderDistance_;

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };
    };
}
