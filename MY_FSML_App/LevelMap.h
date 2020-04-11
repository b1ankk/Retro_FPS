#pragma once

#include <vector>
#include <memory>

#include "EntityVector.h"

namespace game
{
    class MapTile;
    class TileTypeManager;
    class FPP_Player;


    using IntMap = std::vector<std::vector<int>>;
    using TileMap = std::vector<std::vector<MapTile>>;
    using MapDataPtr = std::shared_ptr<const TileMap>;

    class LevelMap
    {
    public:
        LevelMap(const LevelMap&) = delete;
        LevelMap() = default;
        ~LevelMap() = default;

        void loadFromInts(const IntMap&);
        void loadFromTiles(TileMap);


    private:
        MapDataPtr mapData_{};
        EntityVector entities_{};

        // GameObjRef<FPP_Player> player_{GAME::PLAYER};
        
        
    public:

        // GETTERS & SETTERS

        const auto& operator[](const unsigned& row) const
        {
            return (TEST_MAP)[row];
        }

        [[nodiscard]]
        MapDataPtr mapData() const
        {
            return mapData_;
        }

        EntityVector& entities()
        {
            return entities_;
        }


        static const IntMap TEST_MAP;
        
    };
}

