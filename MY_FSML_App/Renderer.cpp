#include <cmath>
#include "Renderer.h"

#include <SFML/Graphics/Image.hpp>
#include <utility>

#include "vector_additions.h"

namespace game
{
    Renderer::Renderer(const FPP_Player&    player,
                       const int&           width,
                       const int&           height,
                       std::unique_ptr<sf::RenderWindow> renderWindow,
                       const SpriteManager& spriteManager,
                       const double&        renderDistance) :
        width_(width),
        height_(height),
        renderWindow_(renderWindow),
        spriteManager_(spriteManager),
        screenBuffer_(new sf::Uint8[4L * width_ * height_]),
        player_(player),
        plane_(player.cameraPlane()),
        renderDistance_(renderDistance)
    {
    }


    Renderer::~Renderer()
    {
        delete[] screenBuffer_;
    }


    void Renderer::drawTexturedWorld()
    {
        for (int x = 0; x < width_; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                player_.direction().x + plane_.x * cameraX,
                player_.direction().y + plane_.y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(player_.position());

            // length of ray from one x or y-side to next x or y-side
            // not a Vector2!!! but separate values
            double deltaDistX = std::abs(1 / rayDirection.x);
            double deltaDistY = std::abs(1 / rayDirection.y);

            // next step direction
            sf::Vector2i stepDirection{};
            // length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            if (rayDirection.x < 0)
            {
                stepDirection.x = -1;
                sideDistX = (player_.position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX = (gridPosition.x + 1.0 - player_.position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY = (player_.position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY = (gridPosition.y + 1.0 - player_.position().y) * deltaDistY;
            }

            bool        hit = false;
            WallHitSide side{WallHitSide::NONE};
            // perform DDA
            while (!hit)
            {
                // jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    gridPosition.x += stepDirection.x;
                    side = WallHitSide::HORIZONTAL;
                }
                else
                {
                    sideDistY += deltaDistY;
                    gridPosition.y += stepDirection.y;
                    side = WallHitSide::VERTICAL;
                }
                // Check if ray has hit a wall
                if (tempWorldMap[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - player_.position().x
                                + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - player_.position().y
                                + (1 - stepDirection.y) / 2) / rayDirection.y;


            //Calculate height of line to draw on screen
            int lineHeight = static_cast<int>(height_ / wallPerpDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawEnd >= height_)
                drawEnd = height_ - 1;

        }


    }

    void Renderer::drawPrimitiveWorld()
    {

        for (int x = 0; x < width_; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                player_.direction().x + plane_.x * cameraX,
                player_.direction().y + plane_.y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(player_.position());

            // length of ray from one x or y-side to next x or y-side
            // not a Vector2!!! but separate values
            double deltaDistX = std::abs(1 / rayDirection.x);
            double deltaDistY = std::abs(1 / rayDirection.y);

            // next step direction
            sf::Vector2i stepDirection{};
            // length of ray from current position to next x or y-side
            double sideDistX;
            double sideDistY;

            if (rayDirection.x < 0)
            {
                stepDirection.x = -1;
                sideDistX = (player_.position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX = (gridPosition.x + 1.0 - player_.position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY = (player_.position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY = (gridPosition.y + 1.0 - player_.position().y) * deltaDistY;
            }

            bool        hit = false;
            WallHitSide side{WallHitSide::NONE};
            // perform DDA
            while (!hit)
            {
                // jump to next map square, OR in x-direction, OR in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    gridPosition.x += stepDirection.x;
                    side = WallHitSide::HORIZONTAL;
                }
                else
                {
                    sideDistY += deltaDistY;
                    gridPosition.y += stepDirection.y;
                    side = WallHitSide::VERTICAL;
                }
                // Check if ray has hit a wall
                if (tempWorldMap[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - player_.position().x
                                + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - player_.position().y
                                + (1 - stepDirection.y) / 2) / rayDirection.y;


            //Calculate height of line to draw on screen
            int lineHeight = static_cast<int>(height_ / wallPerpDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawEnd >= height_)
                drawEnd = height_ - 1;

            //choose wall color
            sf::Color color;

            //TODO change world map handling
            switch (tempWorldMap[gridPosition.x][gridPosition.y])
            {
                case 1:
                    color = sf::Color::Red;
                    break; //red
                case 2:
                    color = sf::Color::Green;
                    break; //green
                case 3:
                    color = sf::Color::Blue;
                    break; //blue
                case 4:
                    color = sf::Color::White;
                    break; //white
                default:
                    color = sf::Color::Yellow;
                    break; //yellow
            }

            //give x and y sides different brightness
            // if (side == 1) { color = color / 2; }
            if (side == WallHitSide::HORIZONTAL)
            {
                color.r /= 2;
                color.g /= 2;
                color.b /= 2;
            }


            //draw the pixels of the stripe as a vertical line
            // verLine(x, drawStart, drawEnd, color);
            sf::Vector2f startVec{static_cast<float>(x), static_cast<float>(drawStart)};
            sf::Vector2f endVec{static_cast<float>(x), static_cast<float>(drawEnd)};
            sf::Vertex   vertices[2]
            {
                sf::Vertex{startVec, color},
                sf::Vertex{endVec, color}
            };


            renderWindow_->draw(vertices, 2, sf::Lines);
        }
    }
}
