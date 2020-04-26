#include "Renderer.h"

#include <cassert>
#include <future>
#include <memory>

#include "Game.h"
#include "UserInterface.h"
#include "UI_Element_Children.h"
#include "FPP_Player.h"
#include "LevelMap.h"
#include "MapTile.h"
#include "Entity.h"

namespace game
{
    Renderer::Renderer(const int&    width,
                       const int&    height,
                       const double& renderDistance) :
        width_(width),
        height_(height),
        screenBufferLength_(width_ * 4LLU * height_),
        screenBuffer_(new sf::Uint8[screenBufferLength_]{0}),
        screenClearBuffer_(new sf::Uint8[screenBufferLength_]{0}),
        renderDistance_(renderDistance),
        vertices_(sf::PrimitiveType::Quads),
        perpWallDistances_(width_),
        screenFilter_(sf::Vector2f{static_cast<float>(width_), static_cast<float>(height_)})
    {
        // std::fill(screenBuffer_, screenBuffer_ + screenBufferLength_, 0xff);

        // prepare cleared buffer for fast copy
        constexpr sf::Uint32 mask  = 0xff303030;
        constexpr sf::Uint32 mask2 = 0xff606060;
        for (sf::Uint8 * ptr    = screenClearBuffer_,
                       * endPtr = screenClearBuffer_ + screenBufferLength_;
             ptr < endPtr;
             ptr += 4)
        {
            if (ptr < screenClearBuffer_ + screenBufferLength_ / 2)
            {
                memcpy(ptr, &mask, 4);
            }
            else
            {
                memcpy(ptr, &mask2, 4);
            }
        }
        std::memcpy(screenBuffer_, screenClearBuffer_, screenBufferLength_);


        for (int i = 0; i <= THREADS; ++i)
        {
            renderThreadBounds_.push_back(width_ * i / THREADS);
        }

        initVertices();

        // Set up render states
        renderStates_.texture = &texture_;
        // renderStates_.transform.scale(1. / scale, 1 / scale);
    }


    Renderer::~Renderer()
    {
        delete[] screenBuffer_;
        delete[] screenClearBuffer_;
    }

    void Renderer::initUI()
    {
        userInterface_ = std::make_shared<UserInterface>(
            width_,
            height_,
            sf::Vector2i{0, 0}
        );

        // auto health = std::make_shared<HealthUIE>(
        //     sf::Vector2i{80, 64},
        //     sf::Vector2i{0, height_ - 64}
        // );

        const int paneWidth = 100;
        const int paneHeight = 64;
        const float scalerX = 3.;
        const float scalerY = 2.5;
        const float scaledWidth = paneWidth * scalerX;
        const float scaledHeight = paneHeight * scalerY;

        auto health = std::make_shared<HealthUIE>(
            sf::Vector2i{paneWidth + 4, paneHeight},
            sf::Vector2i{0, height_ - int(scaledHeight)}
        );
        health->setScale(scalerX, scalerY);
        userInterface_->addUI_Element(
            health
        );

        auto armor = std::make_shared<ArmorUIE>(
            sf::Vector2i{paneWidth - 4, paneHeight},
            sf::Vector2i{static_cast<int>(scaledWidth + 4 * scalerX),
                static_cast<int>(static_cast<float>(height_) - scaledHeight)}
        );
        armor->setScale(scalerX, scalerY);
        userInterface_->addUI_Element(
            armor
        );

        auto ammo = std::make_shared<AmmoUIE>(
            sf::Vector2i{paneWidth, paneHeight},
            sf::Vector2i{static_cast<int>(static_cast<float>(width_) - scaledWidth),
                height_ - int(scaledHeight)}
        );
        ammo->setScale(scalerX, scalerY);
        userInterface_->addUI_Element(
            ammo
        );

        auto fill = std::make_shared<FillerUIE>(
            sf::Vector2i{int(float(width_) / scalerX) - 3 * paneWidth, paneHeight},
            sf::Vector2i{int(2 * scaledWidth), height_ - int(scaledHeight)}
        );
        fill->setScale(scalerX, scalerY);
        userInterface_->addUI_Element(
            fill
        );

        auto gun = std::make_shared<AnimatedGunUIE>(
            sf::Vector2i{160, 160},
            sf::Vector2i{width_ / 2, height_ - int(scaledHeight)}
        );
        gun->setScale(3, 3);
        userInterface_->addUI_Element(gun);

    }


    void Renderer::initVertices()
    {
        vertices_.append(
            sf::Vertex{
                {0, 0},
                sf::Color::White,
                {0, 0}
            }
        );
        vertices_.append(
            sf::Vertex{
                {static_cast<float>(width_), 0},
                sf::Color::White,
                {static_cast<float>(width_), 0}
            }
        );
        vertices_.append(
            sf::Vertex{
                {static_cast<float>(width_), static_cast<float>(height_)},
                sf::Color::White,
                {static_cast<float>(width_), static_cast<float>(height_)}
            }
        );
        vertices_.append(
            sf::Vertex{
                {0, static_cast<float>(height_)},
                sf::Color::White,
                {0, static_cast<float>(height_)}
            }
        );
    }


    void Renderer::renderFrame()
    {
        Game::get().window()->clear(sf::Color{0xff});

        std::vector<std::future<void>> futures;
        futures.reserve(THREADS);
        for (size_t i = 0; i < THREADS; ++i)
        {
            futures.push_back(
                std::async(
                    std::launch::async,
                    [this](const int& start, const int& end) { drawTexturedWorld(start, end); },
                    renderThreadBounds_[i],
                    renderThreadBounds_[i + 1LLU]
                )
            );
        }

        for (const auto& f : futures)
            f.wait();

        frame_.create(width_, height_, screenBuffer_);
        texture_.loadFromImage(frame_);
        // texture.setSrgb(true);
        // texture_.setSmooth(true);


        // sf::RenderStates renderStates;
        // renderStates.texture = &texture_;
        // renderStates.transform.scale(0.5, 0.5);

        // sf::Sprite sprite{texture};
        // // sprite.scale(1, 1);


        Game::get().window()->draw(vertices_, renderStates_);

        drawEntities();

        std::memcpy(screenBuffer_, screenClearBuffer_, screenBufferLength_);

        // TODO Move to more adequate place 
        userInterface_->update();

        Game::get().window()->draw(*userInterface_);

        screenFilter_.setFillColor(filterColor_.color());
        Game::get().window()->draw(screenFilter_);


        if (drawFpsCounter_)
            drawFPS();


        Game::get().window()->display();
    }


    void Renderer::drawTexturedWorld(const int& start, const int& end)
    {
        for (int x = start; x < end; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                Game::get().player()->direction().x + Game::get().player()->cameraPlane().x * cameraX,
                Game::get().player()->direction().y + Game::get().player()->cameraPlane().y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(Game::get().player()->position());

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
                sideDistX       = (Game::get().player()->position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX       = (gridPosition.x + 1.0 - Game::get().player()->position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY       = (Game::get().player()->position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY       = (gridPosition.y + 1.0 - Game::get().player()->position().y) * deltaDistY;
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
                if ((*Game::get().levelMap())[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - Game::get().player()->position().x
                    + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - Game::get().player()->position().y
                    + (1 - stepDirection.y) / 2) / rayDirection.y;

            perpWallDistances_[x] = wallPerpDist;
            //Calculate height of line to draw on screen
            const int lineHeight = static_cast<int>(height_ / wallPerpDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawEnd >= height_)
                drawEnd = height_ - 1;


            const game::MapTile tile = (*Game::get().levelMap()->mapData())[gridPosition.x][gridPosition.y];

            if (tile.typeId() != 0)
            {
                const sf::Image&   image       = tile.image();       // image to use in rendering
                const sf::Image&   shadowImage = tile.shadowImage(); // shadowed image used to imitate shading
                const sf::Vector2i imageSize{
                    static_cast<int>(image.getSize().x),
                    static_cast<int>(image.getSize().y)
                };

                double wallHitX; // where the wall was hit
                if (side == WallHitSide::HORIZONTAL)
                    wallHitX = Game::get().player()->position().y + wallPerpDist * rayDirection.y;
                else
                    wallHitX = Game::get().player()->position().x + wallPerpDist * rayDirection.x;
                wallHitX -= floor(wallHitX);

                int imageX = static_cast<int>(wallHitX * imageSize.x);
                if (side == WallHitSide::HORIZONTAL && rayDirection.x > 0 ||
                    side == WallHitSide::VERTICAL && rayDirection.y < 0)
                    imageX = imageSize.x - imageX - 1;


                const double step = 1.0 * imageSize.y / lineHeight; // image texture increase for screen pixel

                double imageYPos = (drawStart - height_ / 2 + lineHeight / 2) * step;

                int imageY = static_cast<int>(imageYPos) & (imageSize.y - 1);


                sf::Uint8* imagePtr;

                if (side == WallHitSide::VERTICAL)
                {
                    imagePtr =
                            const_cast<sf::Uint8*>(shadowImage.getPixelsPtr()) + imageY * 4LLU * imageSize.x + 4LLU *
                            imageX;
                }
                else
                {
                    imagePtr =
                            const_cast<sf::Uint8*>(image.getPixelsPtr()) + imageY * 4LLU * imageSize.x + 4LLU * imageX;
                }


                sf::Uint8* screenPtr{};

                bool once{true};
                // static size_t imagePtrStep = imageSize.x * 4LLU;
                for (int y = drawStart; y < drawEnd; ++y, screenPtr += 4LLU * width_)
                {
                    if (once)
                    {
                        screenPtr = (screenBuffer_ + y * 4LLU * width_ + x * 4LLU);
                        once      = false;
                    }
                    int old = imageY;
                    imageY  = static_cast<int>(imageYPos) & (imageSize.y - 1);
                    imageYPos += step;
                    // sf::Uint32 color = image.getPixel(imageX, imageY).toInteger();


                    // if (side == WallHitSide::VERTICAL)
                    //     color >>= 1;

                    // *(screenBuffer_ + y * 4LLU * width_ + x * 4LLU) = color >> 24;
                    // *(screenBuffer_ + y * 4LLU * width_ + x * 4LLU + 1) = color >> 16;
                    // *(screenBuffer_ + y * 4LLU * width_ + x * 4LLU + 2) = color >> 8;
                    // *(screenBuffer_ + y * 4LLU * width_ + x * 4LLU + 3) = color;


                    if (old != imageY)
                        imagePtr += imageSize.x * 4LLU * (imageY - old);


                    std::memcpy(
                        (screenPtr),
                        imagePtr,
                        3
                    );
                }
            }
        }


        // int mask = 0xff;
        // for (auto ptr = screenBuffer_,
        //      end = screenBuffer_ + screenBufferLength_;
        //      ptr < end;
        //      ptr += 4)
        // {
        //     std::memcpy(
        //         (ptr),
        //         &mask, 4
        //     );
        // }
    }


    std::pair<double, double> Renderer::findTransitionPoint(double left, double right, const double perpDist)
    {
        if (right < left)
            return {-1, -1};

        const double oldLeft{left};

        double  resultL{-1};
        double  resultR{-1};
        bool foundL{false};
        bool foundR{false};

        if (left < 0)
            left = 0;
        else if (left >= perpWallDistances_.size())
            left = perpWallDistances_.size() - 1.;

        if (right < 0)
            right = 0;
        else if (right >= perpWallDistances_.size())
            right = perpWallDistances_.size() - 1.;

        const double start = left;
        const double end   = right;

        double leftPtr  = start;
        double rightPtr = end;


        while (leftPtr <= end && !foundL)
        {
            if (perpWallDistances_[static_cast<size_t>(leftPtr)] > perpDist)
            {
                resultL = leftPtr - oldLeft - 0.5;
                foundL  = true;
            }
            leftPtr++;
        }

        while (rightPtr >= 0 && !foundR)
        {
            if (perpWallDistances_[static_cast<size_t>(rightPtr)] > perpDist)
            {
                resultR = rightPtr - oldLeft + 0.5;
                foundR  = true;
            }
            rightPtr--;
        }

        if (rightPtr < leftPtr)
            return {-1, -1};

        return std::make_pair(resultL, resultR);
    }

    void Renderer::drawEntities()
    {
        Game::get().levelMap()->entities().updateToPos(Game::get().player()->position());

        const double Dx = Game::get().player()->direction().x;
        const double Dy = Game::get().player()->direction().y;
        const double Px = Game::get().player()->cameraPlane().x;
        const double Py = Game::get().player()->cameraPlane().y;

        for (auto& entity : Game::get().levelMap()->entities())
        {
            const sf::Vector2d relativePosToPlayer = entity->mapPosition() - Game::get().player()->position();

            const double Vx = relativePosToPlayer.x;
            const double Vy = relativePosToPlayer.y;

            const double invDet     = 1.0 / (Px * Dy - Dx * Py);
            const double transformY = invDet * (-Py * Vx + Px * Vy);

            if (transformY > 0)
            {
                const double transformX = invDet * (Dy * Vx - Dx * Vy);
                const double position   = (1 + transformX / transformY) * width_ / 2;
                const double scale      = (height_ / transformY) / 64.;


                entity->resetVertices();

                assert(perpWallDistances_.size() == size_t(width_));
                const double left  = position - (entity->imageSize().x * scale) / 2.;
                const double right = position + (entity->imageSize().x * scale) / 2.;


                const auto borders = findTransitionPoint(
                    left,
                    right,
                    transformY
                );

                bool transformed{false};
                if (borders.first >= 0)
                {
                    entity->vertices()[0] = sf::Vertex{
                        {float((borders.first / scale)), entity->vertices()[0].position.y},
                        entity->vertices()[0].color,
                        {float((borders.first / scale)), entity->vertices()[0].texCoords.y}
                    };

                    entity->vertices()[3] = sf::Vertex{
                        {float((borders.first / scale)), entity->vertices()[3].position.y},
                        entity->vertices()[3].color,
                        {float((borders.first / scale)), entity->vertices()[3].texCoords.y}
                    };

                    transformed = true;
                }
                if (borders.second >= 0)
                {
                    entity->vertices()[1] = sf::Vertex{
                        {float((borders.second / scale)), entity->vertices()[1].position.y},
                        entity->vertices()[1].color,
                        {float((borders.second / scale)), entity->vertices()[1].texCoords.y}
                    };
                    entity->vertices()[2] = sf::Vertex{
                        {float((borders.second / scale)), entity->vertices()[2].position.y},
                        entity->vertices()[2].color,
                        {float((borders.second / scale)), entity->vertices()[2].texCoords.y}
                    };

                    transformed = true;
                }
                if (!transformed) continue;


                entity->setPosition(static_cast<float>(position), 
                                    static_cast<float>(height_ * entity->screenYPosition()));

                entity->setScale(static_cast<float>(scale), static_cast<float>(scale));

                Game::get().window()->draw(*entity);
            }
        }
    }

    sf::Text initFpsText()
    {
        sf::Text text;
        text.setFont(*Game::get().fontManager()->getFontForName("Pixeled"));

        text.setCharacterSize(34);
        // text.setLineSpacing(text.getLineSpacing() * 0.6);
        text.setLineSpacing(0.5);
        text.setFillColor(sf::Color::White);
        text.setOutlineColor(sf::Color::Black);
        text.setOutlineThickness(3);

        return text;
    }

    void Renderer::drawFPS()
    {
        static sf::Text text = initFpsText();

        char s[80];
        snprintf(
            s,
            80,
            "FPS: %d\nx: %2.2f\ny: %2.2f\ndir_x: %g\ndir_y: %g",
            Game::get().fps(),
            Game::get().player()->position().x,
            Game::get().player()->position().y,
            Game::get().player()->direction().x,
            Game::get().player()->direction().y
        );

        text.setString(s);

        sf::RenderStates rs{};
        rs.transform.translate(10, 20);
        Game::get().window()->draw(text, rs);
    }

    void Renderer::drawGameOverScreen()
    {
        screenFilter_.setFillColor(sf::Color::Red);
        sf::Text text{
            "GAME OVER",
            *Game::get().fontManager()->getFontForId(0),
            80u
        };
        text.setFillColor(sf::Color::White);
        text.setOutlineThickness(4);
        text.setOutlineColor(sf::Color::Black);
        text.setPosition(static_cast<float>(width_) / 2.f, static_cast<float>(height_) / 2.f);
        auto boundRectValue = text.getGlobalBounds();
        text.setOrigin(boundRectValue.width / 2, boundRectValue.height / 2);
        
        Game::get().window()->draw(screenFilter_);
        Game::get().window()->draw(text);
        Game::get().window()->display();
    }


    void Renderer::drawPrimitiveWorld()
    {
        Game::get().window()->clear(sf::Color::Black);

        for (int x = 0; x < width_; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                Game::get().player()->direction().x + Game::get().player()->cameraPlane().x * cameraX,
                Game::get().player()->direction().y + Game::get().player()->cameraPlane().y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(Game::get().player()->position());

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
                sideDistX       = (Game::get().player()->position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX       = (gridPosition.x + 1.0 - Game::get().player()->position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY       = (Game::get().player()->position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY       = (gridPosition.y + 1.0 - Game::get().player()->position().y) * deltaDistY;
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
                if ((*Game::get().levelMap())[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - Game::get().player()->position().x
                    + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - Game::get().player()->position().y
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
            switch ((*Game::get().levelMap())[gridPosition.x][gridPosition.y])
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


            Game::get().window()->draw(vertices, 2, sf::Lines);
        }
    }
}
