#pragma once
#include <memory>
#include <string>

#include "SpriteWrapper.h"
#include "TileType.h"

namespace game
{
    class MapTile
    {
    public:
        // TODO MapTile


    private:
        std::shared_ptr<TileType> tileType_;


    public:
        // GETTERS & SETTERS


        bool isIsTraversable() const
        {
            return tileType_->isTraversable();
        }

        
    };

    
}
