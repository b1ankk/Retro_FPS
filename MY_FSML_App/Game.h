#pragma once

#include <memory>
#include <SFML/Graphics/RenderWindow.hpp>

#include "EntityVector.h"
#include "GameObjRef.h"
#include "AssetManager.h"


namespace game
{
    class AssetManager;
    class LevelMap;
    class Renderer;
    class InputHandler;
    class FPP_Player;
    class TileTypeManager;
    class TextureManager;
    class SpriteManager;

    class Game
    {
    public:
        // CONSTANTS

        static const int WINDOW_WIDTH{1920};
        static const int WINDOW_HEIGHT{1080};
        static const sf::Vector2i SCREEN_MIDDLE;

        const sf::Vector2d INITIAL_PLAYER_POS{22, 12};
        const sf::Vector2d INITIAL_PLAYER_DIR{-1, 0};

        const sf::Time SECOND{sf::seconds(1.f)};

        Game(const Game&) = delete;

        void start();

        static Game& get()
        {
            static Game game;

            return game;
        }

    private:
        // CONSTRUCTOR
        Game() = default;

        //////// GAME OBJECTS ////////

        // FIELDS
        int lastFps_{};

        double renderScale_{1.};

        // CONST INSTANCES

        std::shared_ptr<sf::RenderWindow> window_{};
        std::shared_ptr<AssetManager>     assetManager_{};
        std::shared_ptr<Renderer>         renderer_{};
        std::shared_ptr<InputHandler>     inputHandler_{};
        std::shared_ptr<FPP_Player>       player_{};


        // MUTABLE INSTANCES

        std::shared_ptr<LevelMap> levelMap_{};


        // METHODS

        void setUpGame();


    public:

        // GETTERS & SETTERS

        [[nodiscard]]
        GameObjRef<sf::RenderWindow> window() const
        {
            return window_;
        }

        [[nodiscard]]
        GameObjRef<AssetManager> assetManager() const
        {
            return assetManager_;
        }

        [[nodiscard]]
        GameObjRef<SpriteManager> spriteManager() const
        {
            return assetManager_->spriteManager();
        }

        [[nodiscard]]
        GameObjRef<TileTypeManager> tileTypeManager() const
        {
            return assetManager_->tileTypeManager();
        }

        [[nodiscard]]
        GameObjRef<TextureManager> textureManager() const
        {
            return assetManager_->textureManager();
        }

        [[nodiscard]]
        GameObjRef<Renderer> renderer() const
        {
            return renderer_;
        }

        [[nodiscard]]
        GameObjRef<InputHandler> inputHandler() const
        {
            return inputHandler_;
        }

        [[nodiscard]]
        GameObjRef<FPP_Player> player() const
        {
            return player_;
        }

        [[nodiscard]]
        GameObjRef<LevelMap> levelMap() const
        {
            return levelMap_;
        }

        [[nodiscard]]
        const int& fps() const
        {
            return lastFps_;
        }


    };
}


// namespace GAME
// {
//     using namespace game;
//
//     extern Game&                        INSTANCE;
//     extern GameObjRef<LevelMap>         LEVEL_MAP;
//     extern GameObjRef<sf::RenderWindow> WINDOW;
//     extern GameObjRef<AssetManager>     ASSET_MANAGER;
//     extern GameObjRef<SpriteManager>    SPRITE_MANAGER;
//     extern GameObjRef<TileTypeManager>  TILE_TYPE_MANAGER;
//     extern GameObjRef<TextureManager>   TEXTURE_MANAGER;
//     extern GameObjRef<Renderer>         RENDERER;
//     extern GameObjRef<InputHandler>     INPUT_HANDLER;
//     extern GameObjRef<FPP_Player>       PLAYER;
// }
