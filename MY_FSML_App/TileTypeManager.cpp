#include "TileTypeManager.h"

#include <ostream>

namespace game
{
    void TileTypeManager::loadTileTypeInfo(std::string tileTypeName, std::shared_ptr<TileType> tileType)
    {
        tileTypeOrder_.push_back(tileType);

        tileTypesMap_.insert(
            {std::move(tileTypeName), std::move(tileType)}
        );
    }

    void TileTypeManager::loadAllTileTypes(const SpriteManager& spriteManager)
    {
        for (auto& tileTypePair : tileTypesMap_)
        {
            tileTypePair.second->setSprite(
                spriteManager.getSprite(tileTypePair.second->spriteName())
            );
            std::cout << "Loaded tileType: " << tileTypePair.first << std::endl;
        }
    }

}
