#pragma once

#include <memory>

#include "Collidable.h"
#include "TileType.h"
#include "SpriteWrapper.h"
#include "RectangleCollider.h"

namespace game
{
    class MapTile : public Collidable


    {
    public:
        MapTile(std::shared_ptr<const TileType> tileType,
                const sf::Vector2i&             position,
                std::shared_ptr<Collider>       collider) :
            Collidable(std::move(collider)),
            tileType_(std::move(tileType)),
            position(position)
        {
        }

    private:
        std::shared_ptr<const TileType> tileType_;
        sf::Vector2i                    position{};

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
