#pragma once
#include <string>
#include <unordered_map>

#include "SpriteWrapper.h"

namespace game
{
    class SpriteManager
    {

    public:

        /**
     * Loads a sprite's info and saves it into the internal unordered_map
     * @param imageName Name of the sprite
     * @param imagePath Path to the sprite
     */
        void loadSpriteInfo(std::string imageName, std::string imagePath);

        /**
     * Loads all the sprites from files specified in internal game::SpriteWrapper instances
     */
        void loadAllSprites();


    private:
        std::unordered_map<std::string, game::SpriteWrapper> spritesMap_{};


    };

    
}
