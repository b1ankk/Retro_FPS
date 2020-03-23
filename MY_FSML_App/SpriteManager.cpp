#include "SpriteManager.h"

#include <iostream>
#include <ostream>


#include "SpriteWrapper.h"

namespace game
{
    void SpriteManager::loadSpriteInfo(std::string imageName, std::string imagePath)
    {
        SpriteWrapper spriteWrapper{
            std::move(imagePath), std::move(imageName)
        };

        spritesMap_.insert(
            {spriteWrapper.getImageName(), std::move(spriteWrapper)}
        );

    }

    void SpriteManager::loadAllSprites()
    {
        for (auto& sprite : spritesMap_)
        {
            try
            {
                sprite.second.load();
            }
            catch (...)
            {
                std::cerr << "Error loading file: " << sprite.second.getImagePath() << std::endl;
            }
        }
    }
    
}
