#pragma once

#include <rapidjson/document.h>
#include <unordered_map>

#include "TileTypeManager.h"


namespace game
{
    class AssetManager
    {
    public:
        explicit AssetManager(std::string assetInfoFilePath);

        void loadAssets();

    private:
        const std::string assetInfoFilePath_{};
        rapidjson::Document assetInfo_;

        void loadAssetInfo();

        // SPRITE MANAGEMENT

        SpriteManager spriteManager_;
        void loadSpriteInfo();
        void loadSprites();

        // TILE TYPE MANAGEMENT

        TileTypeManager tileTypeManager_;
        void loadTileTypeInfo();
        void loadTileTypes();

    public:
        const SpriteManager& spriteManager() const
        {
            return spriteManager_;
        }


    };

}
