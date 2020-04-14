#pragma once

#include <memory>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Clock.hpp>

#include <utility>
#include <vector>

namespace game
{
    class SpriteWrapper;

    class Animation
    {
    public:
        class AnimationFrame;

        void play();
        void loop();
        void pause();
        void stop();
        void stopRepeating();
        std::shared_ptr<const sf::Texture> texture();

    private:
        std::vector<AnimationFrame> frames_{};
        sf::Clock clock_;
        sf::Time pausedTime_{};
        bool playing_{};
        bool repeating_{};
        int currentFrame_{0};


    public:
        

        void addFrame(AnimationFrame animFrame)
        {
            frames_.push_back(std::move(animFrame));
        }

        class AnimationFrame
        {
        public:
            AnimationFrame(std::shared_ptr<const sf::Texture> texture, const sf::Time& frameEnd)
                : texture_(std::move(texture)),
                  frameEnd_(frameEnd)
            {
            }

        private:
            std::shared_ptr<const sf::Texture> texture_;
            sf::Time frameEnd_{sf::seconds(1)}; // time calculated from the start of the whole animation


        public:
            [[nodiscard]]
            std::shared_ptr<const sf::Texture> texture() const
            {
                return texture_;
            }

            [[nodiscard]]
            const sf::Time& endTime() const
            {
                return frameEnd_;
            }
        };
    };


}

