#pragma once

#include <rapidjson/document.h>
#include <unordered_map>



#include "SpriteManager.h"
#include "SpriteWrapper.h"

namespace game
{
    class AssetManager
    {
    public:
        explicit AssetManager(std::string assetInfoFilePath);


        void loadAssetInfo();
        void loadAssets();

    private:
        const std::string assetInfoFilePath_{};
        rapidjson::Document assetInfo_;


        // SPRITE MANAGEMENT

        SpriteManager spriteManager_;
        void loadSpriteInfo(SpriteManager&);
        void loadSprites();

    };

    

    
}
