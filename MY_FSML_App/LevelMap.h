#pragma once

#include <vector>
#include <memory>


#include "Enemy.h"
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
        void update();


    private:
        MapDataPtr mapData_{};
        sf::Vector2i size_{};
        EntityVector entities_{};
        std::vector < std::shared_ptr<Enemy>> enemies_;

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

        std::vector < std::shared_ptr<Enemy>>& enemies()
        {
            return enemies_;
        }

        void addEnemy(std::shared_ptr<Enemy> enemy)
        {
            enemies_.push_back(enemy);
            entities_.add(std::static_pointer_cast<Entity, Enemy>(enemy));
        }


        sf::Vector2i size() const
        {
            return size_;
        }

        static const IntMap TEST_MAP;
        
    };
}

