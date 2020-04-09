#pragma once

#include <memory>
#include <unordered_map>

#include "GameObjRef.h"


namespace game
{
    class SpriteManager;
    class TileType;

    class TileTypeManager
    {
    public:
        TileTypeManager(const TileTypeManager&) = delete;
        TileTypeManager();
        ~TileTypeManager() = default;


        void loadTileTypeInfo(std::string, std::shared_ptr<game::TileType>);
        void loadAllTileTypes(GameObjRef<SpriteManager>);

    private:

        std::unordered_map<std::string, std::shared_ptr<game::TileType>> tileTypesMap_{};
        std::vector<std::shared_ptr<game::TileType>>                     tileTypeOrder_;


    public:
        [[nodiscard]]
        std::shared_ptr<TileType> getTypeForId(const size_t& id) const;

        [[nodiscard]]
        std::shared_ptr<TileType> getTypeForName(const std::string& name) const;
    };
}
