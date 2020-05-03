#pragma once

#include <memory>
#include <random>
#include <SFML/Graphics/RenderWindow.hpp>

#include "EntityVector.h"
#include "GameObjRef.h"
#include "AssetManager.h"
#include "ConfigLoader.h"
#include "Enemy.h"
#include "Renderer.h"


namespace game
{
    class AssetManager;
    class LevelMap;
    class InputHandler;
    class FPP_Player;
    class TileTypeManager;
    class TextureManager;
    class SpriteManager;
    class UserInterface;

    class Game
    {
    public:
        // CONSTANTS

        static const std::string DEFAULT_FONT_NAME;


        const sf::Vector2d INITIAL_PLAYER_POS{22, 2};
        const sf::Vector2d INITIAL_PLAYER_DIR{-1, 0};

        const sf::Time SECOND{sf::seconds(1.f)};

        // CONSTRUCTORS & STATIC INITIALIZERS

        Game(const Game&) = delete;
        Game(Game&&) = delete;

        void setConfig(const Config* config)
        {
            config_.reset(config);
            screenMiddle_ = {config_->wResWidth / 2, config_->wResHeight / 2};
        }
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

        //CONFIG HOLDER
        std::unique_ptr<const Config> config_{nullptr};

        // BASIC MEMBERS

        std::shared_ptr<sf::RenderWindow> window_{};
        std::shared_ptr<AssetManager>     assetManager_{};
        std::shared_ptr<Renderer>         renderer_{};
        std::shared_ptr<InputHandler>     inputHandler_{};
        std::shared_ptr<FPP_Player>       player_{};

        std::shared_ptr<LevelMap> levelMap_{};

        std::unordered_map<std::string, const Enemy> enemyPatterns_;


        // FIELDS

        sf::Vector2i screenMiddle_;
        int lastFps_{};
        bool gameOver_{false};

        std::mt19937 randEngine_;

        // METHODS

        void setUpGame();
        void initRandEngine();
        void setUpEnemyPatterns();
        void spawnRandomEnemies(int);

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
        GameObjRef<AnimationManager> animationManager() const
        {
            return assetManager_->animationManager();
        }

        [[nodiscard]]
        GameObjRef<FontManager> fontManager() const
        {
            return assetManager_->fontManager();
        }


        [[nodiscard]]
        GameObjRef<Renderer> renderer() const
        {
            return renderer_;
        }

        [[nodiscard]]
        GameObjRef<UserInterface> userInterface() const
        {
            return renderer_->userInterface();
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

        [[nodiscard]]
        const sf::Vector2i& screenMiddle() const
        {
            return screenMiddle_;
        }

        std::mt19937& randEngine()
        {
            return randEngine_;
        }

        void setGameOver()
        {
            gameOver_ = true;
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
