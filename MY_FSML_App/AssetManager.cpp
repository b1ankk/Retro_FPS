#include <fstream>
#include <sstream>
#include <ostream>

#include <rapidjson/document.h>

#include "AssetManager.h"
#include "TextureManager.h"
#include "TileTypeManager.h"
#include "TileType.h"

namespace game
{
    using namespace std;

    AssetManager::AssetManager(string assetInfoFilePath) :
        assetInfoFilePath_(move(assetInfoFilePath)),
        spriteManager_(make_shared<SpriteManager>()),
        tileTypeManager_(make_shared<TileTypeManager>()),
        textureManager_(make_shared<TextureManager>(spriteManager_))
    {
        loadAssetInfo();
    }

    void AssetManager::loadAssetInfo()
    {
        ifstream file{assetInfoFilePath_};

        if (!file.good())
            throw runtime_error{"Error reading file: " + assetInfoFilePath_};

        stringstream buffer;
        buffer << file.rdbuf();
        string fileContents = buffer.str();

        file.close();

        // cout << fileContents << endl;

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
        const char* const subFolders[]{"wall", "entity"};

        for (const char* subFolder : subFolders)
        {
            rapidjson::Value& spriteInfo = assetInfo_["assets"]["sprites"][subFolder];

            assert(spriteInfo.IsArray());

            for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
                 itr != spriteInfo.End(); ++itr)
            {
                spriteManager_->loadSpriteInfo(
                    (*itr)["name"].GetString(),
                    (*itr)["path"].GetString()
                );
            }

            cout << "Sprites' info loaded" << endl;
        }
        
    }

    void AssetManager::loadSprites()
    {
        spriteManager_->loadAllSprites();
    }

    void AssetManager::loadTileTypeInfo()
    {
        rapidjson::Value& spriteInfo = assetInfo_["assets"]["tile_types"];
        assert(spriteInfo.IsArray());

        for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
             itr != spriteInfo.End(); ++itr)
        {
            string tileName{(*itr)["name"].GetString()};
            const shared_ptr<TileType> tileType{
                make_shared<TileType>(
                    tileName,
                    (*itr)["sprite_name"].GetString(),
                    (*itr)["traversable"].GetBool()

                )
            };

            tileTypeManager_->loadTileTypeInfo(tileName, tileType);
        }

        cout << "TileType' info loaded" << endl;
    }

    void AssetManager::loadTileTypes()
    {
        tileTypeManager_->loadAllTileTypes(spriteManager_);
    }




}
