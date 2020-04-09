#pragma once
#include <memory>
#include <iostream>
#include <string>
#include <unordered_map>


namespace game
{
    class SpriteWrapper;


    class SpriteManager
    {
    public:
        SpriteManager() = default;
        ~SpriteManager() = default;
        SpriteManager(const SpriteManager&) = delete;

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

        [[nodiscard]]
        std::shared_ptr<const SpriteWrapper> getSprite(const std::string& name) const
        {
            try
            {
                return spritesMap_.at(name);
            }
            catch (const std::out_of_range& e)
            {
                std::cerr << "Couldn't load sprite: " << name << ", " << e.what() << std::endl;
            }
            return nullptr;
        }


    private:
        std::unordered_map<std::string, const std::shared_ptr<SpriteWrapper>> spritesMap_{};
    };
}
