#include "SpriteWrapper.h"

#include <iostream>
#include <ostream>
#include <utility>

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
        isLoaded_ = true;
        std::cout << "Sprite loaded: " << imageName_ << std::endl;
    }


    
}
