#pragma once

#include "TileType.h"

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
        int typeId() const
        {
            return tileType_->id();
        }

        std::string name() const
        {
            return tileType_->name();
        }

        std::string spriteName() const
        {
            return tileType_->spriteName();
        }

        bool isIsTraversable() const
        {
            return tileType_->isTraversable();
        }

        const sf::Image& image() const
        {
            return tileType_->image()->image();
        }

        const sf::Image& shadowImage() const
        {
            return tileType_->image()->shadowImage();
        }
        
    };

    
}
