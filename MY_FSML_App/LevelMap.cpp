#include "LevelMap.h"

namespace game
{
    void LevelMap::loadFromInts(const IntMap& ints)
    {
        TileMap newTileMap = TileMap(ints.size());
        for (size_t i = 0; i < ints.size(); ++i)
        {
            newTileMap[i].reserve(ints.size());
            for (size_t j = 0; j < ints[i].size(); ++j)
            {
                newTileMap[i].push_back(
                    MapTile{
                        tileTypeManager_.getTypeForId(ints[i][j]),
                        sf::Vector2i{static_cast<int>(j), static_cast<int>(i)}
                    }
                );
            }
        }
    
        mapData_ = std::make_shared<const TileMap>(std::move(newTileMap));
    }

    void LevelMap::loadFromTiles(TileMap map)
    {
        mapData_ = std::make_shared<TileMap>(std::move(map));
    }
}
