#include "Renderer.h"

#include <future>
#include <iostream>
#include <ostream>



#include "FPP_Player.h"
#include "LevelMap.h"
#include "MapTile.h"
#include "EntityVector.h"
#include "Entity.h"

namespace game
{
    Renderer::Renderer(std::shared_ptr<const FPP_Player> player,
                       const int&                        width,
                       const int&                        height,
                       std::shared_ptr<sf::RenderWindow> renderWindow,
                       std::shared_ptr
                       <const SpriteManager>           spriteManager,
                       std::shared_ptr<LevelMap> levelMap,
                       const double&                   renderDistance) :
        width_(width),
        height_(height),
        renderWindow_(std::move(renderWindow)),
        spriteManager_(std::move(spriteManager)),
        screenBufferLength_(width_ * 4LLU * height_),
        screenBuffer_(new sf::Uint8[screenBufferLength_]{0}),
        screenClearBuffer_(new sf::Uint8[screenBufferLength_]{0}),
        player_(std::move(player)),
        plane_(player_->cameraPlane()),
        levelMap_(std::move(levelMap)),
        renderDistance_(renderDistance),
        vertices_(sf::PrimitiveType::Quads)
    {
        // std::fill(screenBuffer_, screenBuffer_ + screenBufferLength_, 0xff);

        // prepare cleared buffer for fast copy
        constexpr sf::Uint32 mask = 0xff303030;
        constexpr sf::Uint32 mask2 = 0xff606060;
        for (sf::Uint8* ptr = screenClearBuffer_,
             *endPtr = screenClearBuffer_ + screenBufferLength_;
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
        // renderStates_.transform.scale(1, 1);
    }


    Renderer::~Renderer()
    {
        delete[] screenBuffer_;
        delete[] screenClearBuffer_;
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
        renderWindow_->clear(sf::Color{0xff});

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
        // texture.setSmooth(true);


        

        // sf::RenderStates renderStates;
        // renderStates.texture = &texture_;
        // renderStates.transform.scale(0.5, 0.5);

        // sf::Sprite sprite{texture};
        // // sprite.scale(1, 1);


        

        renderWindow_->draw(vertices_, renderStates_);

        drawEntities();
        
        std::memcpy(screenBuffer_, screenClearBuffer_, screenBufferLength_);

        if (drawFpsCounter_)
            drawFPS();


        renderWindow_->display();
    }


    void Renderer::drawTexturedWorld(const int& start, const int& end)
    {
        for (int x = start; x < end; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                player_->direction().x + plane_.x * cameraX,
                player_->direction().y + plane_.y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(player_->position());

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
                sideDistX       = (player_->position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX       = (gridPosition.x + 1.0 - player_->position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY       = (player_->position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY       = (gridPosition.y + 1.0 - player_->position().y) * deltaDistY;
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
                if ((*levelMap_)[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - player_->position().x
                    + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - player_->position().y
                    + (1 - stepDirection.y) / 2) / rayDirection.y;


            //Calculate height of line to draw on screen
            const int lineHeight = static_cast<int>(height_ / wallPerpDist);

            //calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawStart < 0)
                drawStart = 0;
            int drawEnd = lineHeight / 2 + height_ / 2 /*+ static_cast<int>(verticalOffset)*/;
            if (drawEnd >= height_)
                drawEnd = height_ - 1;


            const game::MapTile tile = (*levelMap_->mapData())[gridPosition.x][gridPosition.y];

            if (tile.typeId() != 0)
            {
                const sf::Image& image = tile.image(); // image to use in rendering
                const sf::Image& shadowImage = tile.shadowImage(); // shadowed image used to imitate shading
                const sf::Vector2i imageSize{
                    static_cast<int>(image.getSize().x),
                    static_cast<int>(image.getSize().y)
                };

                double wallHitX; // where the wall was hit
                if (side == WallHitSide::HORIZONTAL)
                    wallHitX = player_->position().y + wallPerpDist * rayDirection.y;
                else
                    wallHitX = player_->position().x + wallPerpDist * rayDirection.x;
                wallHitX -= floor(wallHitX);

                int imageX = static_cast<int>(wallHitX * imageSize.x);
                if (side == WallHitSide::HORIZONTAL && rayDirection.x > 0 ||
                    side == WallHitSide::VERTICAL && rayDirection.y < 0)
                    imageX = imageSize.x - imageX - 1;


                const double step = 1.0 * imageSize.y / lineHeight; // image texture increase for screen pixel

                double imageYPos = (drawStart - height_ / 2 + lineHeight / 2) * step;

                int imageY = static_cast<int>(imageYPos)& (imageSize.y - 1);

                

                sf::Uint8* imagePtr;

                if (side == WallHitSide::VERTICAL)
                {
                    imagePtr =
                        const_cast<sf::Uint8*>(shadowImage.getPixelsPtr()) + imageY * 4LLU * imageSize.x + 4LLU * imageX;
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
                        once = false;
                    }
                    int old = imageY;
                    imageY = static_cast<int>(imageYPos) & (imageSize.y - 1);
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
                        imagePtr, 3
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

    void Renderer::drawEntities()
    {
        levelMap_->entities().updateToPos(player_->position());
        const sf::Vector2d sightBoundRight = player_->direction() + plane_;
        const sf::Vector2d sightBoundLeft = player_->direction() - plane_;
        // std::cout << player_->position().x << " " << player_->position().y << std::endl;

        for (auto& entity : levelMap_->entities())
        {
            const sf::Vector2d relativePosToPlayer = entity->mapPosition() - player_->position();

            // value describing on what side of the left screen bound is the entity, and how much
            double relToLeft = sightBoundLeft.x * -relativePosToPlayer.y +
                               sightBoundLeft.y * relativePosToPlayer.x;
            // value describing on what side of the right screen bound is the entity, and how much
            double relToRight = sightBoundRight.x * -relativePosToPlayer.y +
                                sightBoundRight.y * relativePosToPlayer.x;

            if ((relToLeft >= -1 && relToRight <= 1))
            {
                // std::cout << "On screen space!" << std::endl;
                
                
                
                // entity->scale(1 / entity->distanceToPlayer(), 1 / entity->distanceToPlayer());
                entity->setOrigin(32, 16);
                //
                // double invDet = 1.0 / (plane_.x * player_->direction().y - player_->direction().x * plane_.y); //required for correct matrix multiplication
                //
                // double transformX = invDet * (player_->direction().y * entity->mapPosition().x - player_->direction().x * entity->mapPosition().y);
                // double transformY = invDet * (-plane_.y * entity->mapPosition().x + plane_.x * entity->mapPosition().y); //this is actually the depth inside the screen, that what Z is in 3D
                //
                // int spriteScreenX = int((width_ / 2) * (1 + transformX / transformY));
                // // int spriteHeight = abs(int(height_ / (transformY))); //using 'transformY' instead of the real distance prevents fisheye
                //
                // entity->setPosition(spriteScreenX, height_ / 2);

                const double Dx = player_->direction().x;
                const double Dy = player_->direction().y;
                const double Px = plane_.x;
                const double Py = plane_.y;
                const double Vx = relativePosToPlayer.x;
                const double Vy = relativePosToPlayer.y;
                //
                // double x = Vx * (Dx * Py - Dy * Px) / (Py * Vx - Px * Vy);
                // double y = (Vy / Vx) * x;
                //
                // double dist = sqrt(pow(-Px + Dx - x, 2) + pow(-Py + Dy - y, 2));
                // double P_length = sqrt(Px * Px + Py * Py);
                // double position = (dist / (2 * P_length)) * width_;
                //
                // if (relToLeft < 0)
                //     position = -position;

                const double invDet = 1.0 / (Px * Dy - Dx * Py);
                const double transformY = invDet * (-Py * Vx + Px * Vy);
                if (transformY < 0)
                    continue;
                const double transformX = invDet * (Dy * Vx - Dx * Vy);
               
                const double position = (1 + transformX / transformY) * width_ / 2;

                entity->setPosition(position, height_ / 2);


                // double perpDistance = sqrt(pow(x - Vx, 2) + pow(y - Vy, 2));
                // const double perpDistance = (-Py * Vx + Px * Vy) / (Px * Dy - Dx * Py);
                entity->setScale(((height_ / transformY) / 64.),
                                 (height_ / transformY) / 64.);
                // entity->setScale(height_ / sqrt(entity->distanceToPlayer()) / 64.,
                // height_ / sqrt(entity->distanceToPlayer()) / 64.);

                
                renderWindow_->draw(*entity);


                // auto sum = plane_ + player_->direction();
                // auto myPerpDif = relativePosToPlayer - player_->direction();
                // double len = sum.x * sum.x + sum.y * sum.y;
                // double x = normalizeVector2(relativePosToPlayer).x * len;
                // entity->setPosition(width_ / 2, height_ / 2);
                // // entity->setScale(width_ / entity->distanceToPlayer(), width_ / entity->distanceToPlayer());
                // renderWindow_->draw(*entity);
            }
        }
    }


    void Renderer::drawFPS()
    {
        //TODO better text rendering implementation

        static std::unique_ptr<sf::Font> font{nullptr};
        static sf::Text text;

        if (font == nullptr)
        {
            font = std::make_unique<sf::Font>(sf::Font{});
            if (!font->loadFromFile("assets\\fonts\\Pixeled.ttf"))
            {
                std::cerr << "Error occured trying to read font: assets\\fonts\\VCR_OSD_MONO_1.001.tff";
                throw std::runtime_error("Error occured trying to read font: assets\\fonts\\VCR_OSD_MONO_1.001.tff");
            }
            text.setFont(*font);
            text.setCharacterSize(34);
            text.setFillColor(sf::Color::White);
        }

        char s[80];
        snprintf(s, 80,"FPS: %d\nx: %2.2f\ny: %2.2f\ndir_x: %g\ndir_y: %g",
                 *fpsCounter_,
                 player_->position().x, player_->position().y,
                 player_->direction().x, player_->direction().y);

        // text.setString("FPS: " + std::to_string(*fpsCounter_) +
        //                "\nx: " + std::to_string(player_->position().x) +
        //                "  y: " + std::to_string(player_->position().y) +
        //                "\ndir_x: " + std::to_string(player_->direction().x) +
        //                "\ndir_y: " + std::to_string(player_->direction().y));

        text.setString(s);
        
        
        sf::RenderStates rs{};
        rs.transform.translate(10, 20);
        renderWindow_->draw(text, rs);
    }
    

    void Renderer::drawPrimitiveWorld()
    {
        renderWindow_->clear(sf::Color::Black);

        for (int x = 0; x < width_; ++x)
        {
            // calculate ray position and direction
            double cameraX = 2. * x / static_cast<double>(width_) - 1.; //x-coordinate in camera space

            sf::Vector2d rayDirection{
                player_->direction().x + plane_.x * cameraX,
                player_->direction().y + plane_.y * cameraX
            };

            // get player's position on the grid
            sf::Vector2i gridPosition = static_cast<sf::Vector2i>(player_->position());

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
                sideDistX       = (player_->position().x - gridPosition.x) * deltaDistX;
            }
            else
            {
                stepDirection.x = 1;
                sideDistX       = (gridPosition.x + 1.0 - player_->position().x) * deltaDistX;
            }
            if (rayDirection.y < 0)
            {
                stepDirection.y = -1;
                sideDistY       = (player_->position().y - gridPosition.y) * deltaDistY;
            }
            else
            {
                stepDirection.y = 1;
                sideDistY       = (gridPosition.y + 1.0 - player_->position().y) * deltaDistY;
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
                if ((*levelMap_)[gridPosition.x][gridPosition.y] > 0)
                    hit = true;
            }

            double wallPerpDist;
            // Calculate distance projected on camera direction (Euclidean distance will give fisheye effect!)
            if (side == WallHitSide::HORIZONTAL)
                wallPerpDist = (gridPosition.x - player_->position().x
                    + (1 - stepDirection.x) / 2) / rayDirection.x;
            else
                wallPerpDist = (gridPosition.y - player_->position().y
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
            switch ((*levelMap_)[gridPosition.x][gridPosition.y])
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
