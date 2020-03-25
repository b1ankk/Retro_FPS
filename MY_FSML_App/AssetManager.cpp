#include <fstream>
#include <sstream>
#include <ostream>

#include <rapidjson/document.h>

#include "AssetManager.h"


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

        loadSpriteInfo();
        loadTileTypeInfo();
    }


    void AssetManager::loadAssets()
    {
        loadSprites();
        loadTileTypes();
    }

    void AssetManager::loadSpriteInfo()
    {
        rapidjson::Value& spriteInfo = assetInfo_["assets"]["sprites"];
        assert(spriteInfo.IsArray());

        for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
             itr != spriteInfo.End(); ++itr)
        {
            spriteManager_.loadSpriteInfo(
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

    void AssetManager::loadTileTypeInfo()
    {
        rapidjson::Value& spriteInfo = assetInfo_["assets"]["tile_types"];
        assert(spriteInfo.IsArray());

        for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
             itr != spriteInfo.End(); ++itr)
        {
            std::string tileName{(*itr)["name"].GetString()};
            const std::shared_ptr<TileType> tileType{
                std::make_shared<TileType>(
                    tileName,
                    (*itr)["sprite_name"].GetString(),
                    (*itr)["traversable"].GetBool()

                )
            };

            tileTypeManager_.loadTileTypeInfo(tileName, tileType);
        }

        std::cout << "TileType' info loaded" << std::endl;
    }

    void AssetManager::loadTileTypes()
    {
        tileTypeManager_.loadAllTileTypes(spriteManager_);
    }




}
