#pragma once

#include <rapidjson/document.h>

#include "SpriteManager.h"
#include "TileTypeManager.h"


namespace game
{
    class TextureManager;




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

        std::shared_ptr<game::SpriteManager> spriteManager_{
            std::make_shared<game::SpriteManager>(
                game::SpriteManager{}
            )
        };
        void loadSpriteInfo();
        void loadSprites();

        // TILE TYPE MANAGEMENT

        std::shared_ptr<game::TileTypeManager> tileTypeManager_{
            std::make_shared<game::TileTypeManager>(
                game::TileTypeManager{}
            )
        };
        void loadTileTypeInfo();
        void loadTileTypes();

        // TEXTURE MANAGEMENT

        std::shared_ptr<game::TextureManager> textureManager_;


    public:
        std::shared_ptr<const game::SpriteManager> spriteManager() const
        {
            return spriteManager_;
        }

        std::shared_ptr<const game::TileTypeManager> tileTypeManager() const
        {
            return tileTypeManager_;
        }

        std::shared_ptr<game::TextureManager> textureManager() const
        {
            return textureManager_;
        }

    };

}
