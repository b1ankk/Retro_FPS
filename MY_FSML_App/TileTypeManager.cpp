#include "TileTypeManager.h"

#include <iostream>

#include "SpriteManager.h"
#include "TileType.h"

namespace game
{
    TileTypeManager::TileTypeManager()
    {
        TileType voidTileType{
            "void_tile",
            "none",
            true
        };
        voidTileType.setSprite(nullptr);

        std::shared_ptr<TileType> voidTileTypePtr{
            std::make_shared<TileType>(
                std::move(voidTileType)
            )
        };

        tileTypeOrder_.push_back(voidTileTypePtr);
        tileTypesMap_.insert({"none", std::move(voidTileTypePtr)});
    }

    void TileTypeManager::loadTileTypeInfo(std::string tileTypeName, std::shared_ptr<TileType> tileType)
    {
        tileTypeOrder_.push_back(tileType);

        tileTypesMap_.insert(
            {std::move(tileTypeName), std::move(tileType)}
        );
    }

    void TileTypeManager::loadAllTileTypes(const std::shared_ptr<const game::SpriteManager>& spriteManager)
    {
        for (auto& tileTypePair : tileTypesMap_)
        {
            tileTypePair.second->setSprite(
                spriteManager->getSprite(tileTypePair.second->spriteName())
            );
            std::cout << "Loaded tileType: " << tileTypePair.first << std::endl;
        }
    }

    std::shared_ptr<TileType> TileTypeManager::getTypeForId(const size_t& id) const
    {
        if (id < tileTypeOrder_.size())
            return tileTypeOrder_[id];

        throw std::out_of_range{
            "No TileType found, for id: "
            + std::to_string(id)
        };
    }

    std::shared_ptr<TileType> TileTypeManager::getTypeForName(const std::string& name) const
    {
        try
        {
            return tileTypesMap_.at(name);
        }
        catch (const std::out_of_range&)
        {
            throw std::out_of_range("No TileType found, for name: " + name);
        }
    }
}
