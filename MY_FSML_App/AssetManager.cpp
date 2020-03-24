#include <fstream>
#include <sstream>
#include <string>
#include <rapidjson/document.h>

#include "AssetManager.h"
#include <iostream>
#include <memory>
#include <ostream>
#include <utility>



namespace game
{
    AssetManager::AssetManager(std::string assetInfoFilePath) :
        assetInfoFilePath_(std::move(assetInfoFilePath))
    {
        loadAssetInfo();
    }

    void AssetManager::loadAssetInfo()
    {
        std::ifstream file{assetInfoFilePath_};

        if (!file.good())
            throw std::runtime_error{"Error reading file: " + assetInfoFilePath_};

        std::stringstream buffer;
        buffer << file.rdbuf();
        std::string fileContents = buffer.str();

        file.close();

        // std::cout << fileContents << std::endl;

        assetInfo_.Parse(fileContents.c_str());

        loadSpriteInfo(this->spriteManager_);
    }


    void AssetManager::loadAssets()
    {
        loadSprites();
    }

    void AssetManager::loadSpriteInfo(SpriteManager& spriteManager)
    {
        rapidjson::Value& spriteInfo = assetInfo_["assets"]["sprites"];
        assert(spriteInfo.IsArray());

        for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
             itr != spriteInfo.End(); ++itr)
        {
            spriteManager.loadSpriteInfo(
                (*itr)["name"].GetString(),
                (*itr)["path"].GetString()
            );
        }

        std::cout << "Sprites' info loaded" << std::endl;
    }

    void AssetManager::loadSprites()
    {
        spriteManager_.loadAllSprites();
    }
}
