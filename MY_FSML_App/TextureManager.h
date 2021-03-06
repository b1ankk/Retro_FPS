#pragma once
#include <memory>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Texture.hpp>


#include "GameObjRef.h"
#include "SpriteWrapper.h"
#include "SpriteManager.h"

namespace game
{
    

    // If need be fetches the image form SpriteManager, converts it to a texture, saves it internally and returns a sharedPointer to it.
    class TextureManager
    {
    public:
        TextureManager(const TextureManager&) = delete;
        ~TextureManager() = default;
        explicit TextureManager(GameObjRef<SpriteManager> spriteManager)
            : spriteManager_(spriteManager)
        {

        }

        std::shared_ptr<const sf::Texture> getTextureForName(const std::string& name)
        {
            const auto seeker = textures_.find(name);
            if (seeker != textures_.end())
            {
                return seeker->second;
            }
            
            std::shared_ptr<const game::SpriteWrapper> sprite = spriteManager_->getSprite(name);
            std::shared_ptr<sf::Texture> texture{std::make_shared<sf::Texture>()};
            texture->loadFromImage(sprite->image());

            textures_.insert(
                {name, texture}
            );

            return texture;
        }


    private:
        GameObjRef<SpriteManager> spriteManager_;

        std::unordered_map<std::string, std::shared_ptr<const sf::Texture>> textures_;

    };

    
}
