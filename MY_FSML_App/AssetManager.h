#pragma once

#include <rapidjson/document.h>
#include <unordered_map>


#include "ImageWrapper.h"

namespace game
{
    class AssetManager
    {
    public:
        explicit AssetManager(std::string assetInfoFilePath);


        void loadAssetInfo();

    private:
        const std::string assetInfoFilePath_{};
        rapidjson::Document assetInfo_;

        
        std::unordered_map<std::string, ImageWrapper> sprites_{};
        void loadSpriteInfo();

    };

    

    
}
