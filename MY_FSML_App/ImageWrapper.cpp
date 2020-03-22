#include "ImageWrapper.h"
#include <utility>

namespace game
{

    ImageWrapper::ImageWrapper(std::string imagePath,
                               std::string imageName) :
        imagePath_(std::move(imagePath)),
        imageName_(std::move(imageName))
    {
    }

    // Load the image from the specified file
    void ImageWrapper::load()
    {
        image_.loadFromFile(imagePath_);
        isLoaded_ = true;
    }


    
}
