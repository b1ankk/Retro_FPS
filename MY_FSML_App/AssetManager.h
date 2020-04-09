#pragma once

#include <rapidjson/document.h>

#include "GameObjRef.h"



namespace game
{
    class SpriteManager;
    class TileTypeManager;
    class TextureManager;
    



    class AssetManager
    {
    public:
        AssetManager(const AssetManager&) = delete;
        explicit AssetManager(std::string assetInfoFilePath);
        ~AssetManager() = default;

        void loadAssets();

    private:
        const std::string assetInfoFilePath_{};
        rapidjson::Document assetInfo_{};

        void loadAssetInfo();

        // SPRITE MANAGEMENT

        const std::shared_ptr<SpriteManager> spriteManager_;
        void loadSpriteInfo();
        void loadSprites();

        // TILE TYPE MANAGEMENT

        const std::shared_ptr<TileTypeManager> tileTypeManager_;
        void loadTileTypeInfo();
        void loadTileTypes();

        // TEXTURE MANAGEMENT

        const std::shared_ptr<TextureManager> textureManager_;


    public:

        // GETTERS & SETTERS

        [[nodiscard]]
        GameObjRef<SpriteManager> spriteManager() const
        {
            return spriteManager_;
        }

        [[nodiscard]]
        GameObjRef<TileTypeManager> tileTypeManager() const
        {
            return tileTypeManager_;
        }

        [[nodiscard]]
        GameObjRef<TextureManager> textureManager() const
        {
            return textureManager_;
        }

    };

}
