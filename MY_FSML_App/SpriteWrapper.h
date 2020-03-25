#pragma once

#include <SFML/Graphics/Image.hpp>

namespace game
{
    class SpriteWrapper
    {
    public:
        SpriteWrapper(std::string imagePath,
                      std::string imageName);

        void load();
    private:

        // FIELDS

        const std::string imagePath_;
        const std::string imageName_;
        sf::Image         image_{};

        // FLAGS
        bool isLoaded_{}; // whether the image was loaded

    public:
        //  GETTERS & SETTERS


        std::string getImagePath() const
        {
            return imagePath_;
        }

        std::string getImageName() const
        {
            return imageName_;
        }

        /**
         * @return a const reference to the instance's image
         */
        const sf::Image& getImage() const
        {
            return image_;
        }

        bool isLoaded() const
        {
            return isLoaded_;
        }
    };
    
}

