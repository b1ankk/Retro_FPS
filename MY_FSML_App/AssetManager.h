#pragma once

#include <rapidjson/document.h>
#include <unordered_map>

#include "SpriteManager.h"


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
        void loadSpriteInfo(SpriteManager&);
        void loadSprites();


    public:
        const SpriteManager& spriteManager() const
        {
            return spriteManager_;
        }


    };

}
