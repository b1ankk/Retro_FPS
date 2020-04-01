#pragma once

#include <memory>
#include <unordered_map>




namespace game
{
    class SpriteManager;
    class TileType;

    class TileTypeManager
    {
    public:
        TileTypeManager();


        void loadTileTypeInfo(std::string, std::shared_ptr<game::TileType>);
        void loadAllTileTypes(const std::shared_ptr<const SpriteManager>&);

    private:

        std::unordered_map<std::string, std::shared_ptr<game::TileType>> tileTypesMap_{};
        std::vector<std::shared_ptr<game::TileType>>                     tileTypeOrder_;


    public:
        std::shared_ptr<TileType> getTypeForId(const size_t& id) const;

        std::shared_ptr<TileType> getTypeForName(const std::string& name) const;
    };
}
