#pragma once
#include <memory>
#include <ostream>
#include <unordered_map>

#include "SpriteManager.h"
#include "TileType.h"

namespace game
{
    class TileTypeManager
    {
    public:

        void loadTileTypeInfo(std::string, std::shared_ptr<game::TileType>);
        void loadAllTileTypes(const SpriteManager&);

    private:

        std::unordered_map<std::string, std::shared_ptr<game::TileType>> tileTypesMap_{};
        std::vector<std::shared_ptr<game::TileType>> tileTypeOrder_;


    public:
        std::shared_ptr<TileType> getTypeForId(const size_t& id)
        {
            if (id < tileTypeOrder_.size())
                return tileTypeOrder_[id];

            throw std::out_of_range{"No TileType found, for id: "
                + std::to_string(id)};
        }

        std::shared_ptr<TileType> getTypeForName(const std::string& name)
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



    };
}
