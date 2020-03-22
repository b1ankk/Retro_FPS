#include <fstream>
#include <sstream>
#include <string>
#include <rapidjson/document.h>

#include "AssetManager.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>

#include "ImageLoader.h"

namespace game
{
    AssetManager::AssetManager(std::string assetInfoFilePath) :
        assetInfoFilePath_(std::move(assetInfoFilePath))
    {
    }

    void AssetManager::loadAssetInfo()
    {
        std::ifstream     file{assetInfoFilePath_};
        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContents = buffer.str();

        file.close();

        // std::cout << fileContents << std::endl;

        assetInfo_.Parse(fileContents.c_str());

        loadSpriteInfo();
    }


    void AssetManager::loadSpriteInfo()
    {
        rapidjson::Value& spriteInfo = assetInfo_["assets"]["sprites"];
        assert(spriteInfo.IsArray());

        for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
             itr != spriteInfo.End(); ++itr)
        {
            ImageWrapper image{
                (*itr)["name"].GetString(),
                (*itr)["path"].GetString()
            };

            sprites_.insert({image.getImageName(), std::move(image)});
        }

        std::cout << "Sprites info loaded" << std::endl;
    }
}
