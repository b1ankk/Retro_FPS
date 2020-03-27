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

        void renderFrame()
        {
            renderWindow_->clear(sf::Color{0xff});

            std::vector<std::future<void>> futures;
            futures.reserve(THREADS);
            for (int i = 0; i < THREADS; ++i)
            {
                futures.push_back(
                    std::async(std::launch::async,
                    [this] (const int& start, const int& end){drawTexturedWorld(start, end); },
                    renderThreadBounds_[i], 
                    renderThreadBounds_[i + 1])
                );
            }

            for (const auto& f : futures)
                f.wait();

            sf::Image frame;
            frame.create(width_, height_, screenBuffer_);
            sf::Texture texture;
            // texture.setSrgb(true);
            texture.loadFromImage(frame);
            sf::Sprite sprite{texture};
            sprite.scale(1, 1);


            renderWindow_->draw(sprite);


            std::memcpy(screenBuffer_, screenClearBuffer_, screenBufferLength_);


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

        std::shared_ptr<const FPP_Player> player_;   // player object 
        const sf::Vector2d&               plane_;    // camera plane
        std::shared_ptr<const LevelMap>   levelMap_; // map of the level

        double renderDistance_;

        // ENUMS & CONSTANTS

        enum class WallHitSide { NONE, VERTICAL, HORIZONTAL };
        std::vector<int> renderThreadBounds_;
        // METHODS
        

        void drawPrimitiveWorld();

        void drawTexturedWorld(const int& start, const int& end);
    };
}
