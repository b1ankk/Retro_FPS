#include "SpriteWrapper.h"

#include <iostream>

namespace game
{
    SpriteWrapper::SpriteWrapper(std::string imagePath,
                                 std::string imageName) :
        imagePath_(std::move(imagePath)),
        imageName_(std::move(imageName))
    {
    }

    // Load the image from the specified file
    void SpriteWrapper::load()
    {
        image_.loadFromFile(imagePath_);
        shadowImage_ = image_;
        putShadow(shadowImage_, 16);

        isLoaded_ = true;
        std::cout << "Sprite loaded: " << imageName_ << std::endl;
    }

    void SpriteWrapper::putShadow(sf::Image& image, const sf::Uint8& strength)
    {
        sf::Uint8* pixelsPtr = const_cast<sf::Uint8*>(image.getPixelsPtr());
        for (sf::Uint8* endPtr = pixelsPtr + image.getSize().x * 4LLU * image.getSize().y; 
             pixelsPtr < endPtr;
             pixelsPtr += 4)
        {
            for(int i = 0; i < 3; i++)
            {
                int difference = pixelsPtr[i] - strength;
                if (difference >= 0)
                    pixelsPtr[i] = difference;
                else
                    pixelsPtr[i] = 0u;
            }
           

        }
    }

}
