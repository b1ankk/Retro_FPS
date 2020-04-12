#include <fstream>
#include <sstream>
#include <ostream>

#include <rapidjson/document.h>

#include "AssetManager.h"
#include "TextureManager.h"
#include "TileTypeManager.h"
#include "TileType.h"
#include "AnimationManager.h"
#include "Game.h"

namespace game
{
    using namespace std;

    AssetManager::AssetManager(string assetInfoFilePath) :
        assetInfoFilePath_(move(assetInfoFilePath)),
        spriteManager_(make_shared<SpriteManager>()),
        tileTypeManager_(make_shared<TileTypeManager>()),
        textureManager_(make_shared<TextureManager>(spriteManager_)),
        animationManager_(make_shared<AnimationManager>())
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
        loadAnimations();
    }

    void AssetManager::loadSpriteInfo()
    {
        const char* const subFolders[]{"wall", "entity"};

        for (const char* subFolder : subFolders)
        {
            rapidjson::Value& spriteInfo = assetInfo_["assets"]["sprites"][subFolder];

            assert(spriteInfo.IsArray());
            
            for (rapidjson::Value::ConstValueIterator itr = spriteInfo.Begin();
                 itr < spriteInfo.End(); ++itr)
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
             itr < spriteInfo.End(); ++itr)
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

    void AssetManager::loadAnimations()
    {
        const char* const subFolders[]{"enemy"};

        for (const char* subFolder : subFolders)
        {
            rapidjson::Value& animationInfo = assetInfo_["assets"]["animations"][subFolder];
        
            assert(animationInfo.IsArray());
        
            for (rapidjson::Value::ConstValueIterator itr = animationInfo.Begin();
                 itr < animationInfo.End(); ++itr)
            {
                string itemName = (*itr)["name"].GetString();
                auto& anims = (*itr)["anims"];

                assert(anims.IsArray());

                for (rapidjson::Value::ConstValueIterator animItr = anims.Begin();
                     animItr != anims.End(); ++animItr)
                {
                    string animName = itemName + "_" + (*animItr)["anim_name"].GetString();
                    auto& framesInfo = (*animItr)["frames"];

                    assert(framesInfo.IsArray());

                    Animation animation{};

                    for (rapidjson::Value::ConstValueIterator frameItr = framesInfo.Begin();
                         frameItr != framesInfo.End(); ++frameItr)
                    {
                        string frameName = (*frameItr)["frame_name"].GetString();
                        sf::Time time = sf::seconds((*frameItr)["time"].GetFloat());

                        Animation::AnimationFrame animFrame{
                            textureManager_->getTextureForName(frameName),
                            time
                        };

                        animation.addFrame(std::move(animFrame));
                    }

                    animationManager_->addAnimation(std::move(animName), std::move(animation));

                }

            }
        
            cout << "Animations' info loaded" << endl;
        }
    }


}
