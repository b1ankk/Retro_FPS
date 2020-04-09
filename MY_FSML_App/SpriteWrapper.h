#pragma once
#include <SFML/Graphics/Image.hpp>
#include <string>


namespace game
{
    class SpriteWrapper
    {
    public:
        // SpriteWrapper(const SpriteWrapper&) = delete;
        ~SpriteWrapper() = default;
        SpriteWrapper(std::string imagePath,
                      std::string imageName);

        void load();
    private:

        // FIELDS

        const std::string imagePath_;
        const std::string imageName_;
        sf::Image         image_{};
        sf::Image         shadowImage_{};

        // FLAGS
        bool isLoaded_{}; // whether the image was loaded

        // METHODS

        void putShadow(sf::Image&, const sf::Uint8&);

    public:
        //  GETTERS & SETTERS


        [[nodiscard]]
        std::string getImagePath() const
        {
            return imagePath_;
        }

        [[nodiscard]]
        std::string getImageName() const
        {
            return imageName_;
        }

        /**
         * @return a const reference to the instance's image
         */
        [[nodiscard]]
        const sf::Image& image() const
        {
            return image_;
        }

        [[nodiscard]]
        const sf::Image& shadowImage() const
        {
            return shadowImage_;
        }

        [[nodiscard]]
        bool isLoaded() const
        {
            return isLoaded_;
        }
    };
    
}

