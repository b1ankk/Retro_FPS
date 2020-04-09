#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "TileType.h"
#include "SpriteWrapper.h"

namespace game
{

    class MapTile
    {
    public:
        MapTile(std::shared_ptr<const TileType> tileType,
                const sf::Vector2i& position)
            : tileType_(std::move(tileType)),
              position(position)
        {
        }

    private:
        std::shared_ptr<const TileType> tileType_;
        sf::Vector2i position{};

    public:
        // GETTERS & SETTERS

        [[nodiscard]]
        int typeId() const
        {
            return tileType_->id();
        }

        [[nodiscard]]
        std::string name() const
        {
            return tileType_->name();
        }

        [[nodiscard]]
        std::string spriteName() const
        {
            return tileType_->spriteName();
        }

        [[nodiscard]]
        bool isTraversable() const
        {
            return tileType_->isTraversable();
        }

        [[nodiscard]]
        const sf::Image& image() const
        {
            return tileType_->image()->image();
        }

        [[nodiscard]]
        const sf::Image& shadowImage() const
        {
            return tileType_->image()->shadowImage();
        }
        
    };

    
}
