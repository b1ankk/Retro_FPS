#include "Game.h"

#include <thread>



#include "AnimatedEntity.h"
#include "Renderer.h"
#include "LevelMap.h"
#include "AssetManager.h"
#include "AnimationManager.h"
#include "FPP_Player.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "Entity.h"
#include "GameTime.h"

namespace game
{
    using namespace std;

    const sf::Vector2i Game::SCREEN_MIDDLE{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    const int Game::RENDERING_WIDTH{WINDOW_WIDTH};
    const int Game::RENDERING_HEIGHT{WINDOW_HEIGHT};

    void Game::start()
    {
        setUpGame();

        int fpsCounter{0};

        // TODO switching FPS counter
        renderer_->setDrawFpsCounter(true);

        double targetFPS{300.};

        sf::Time targetFrameTime{
            sf::microseconds(static_cast<sf::Int64>(1'000'000. / targetFPS))
        };
        sf::Time lastFrameTime;
        game::GameTime::deltaTime_ = lastFrameTime.asMicroseconds() / 1000.f;

        sf::Clock fpsClock;
        sf::Clock frameClock;

        sf::Time toFrameEnd;
        while (window_->isOpen())
        {
            // it is the most safe to handle window events in the function of creation
            sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
            while (window_->pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window_->close();
                if (event.type == sf::Event::KeyPressed)
                {
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window_->close();
                        exit(0);
                    }
                    if (event.key.code == sf::Keyboard::F3)
                    {
                        renderer_->setDrawFpsCounter(!renderer_->ifDrawFpsCounter());
                    }
                }
            }
            inputHandler_->handleInput();

            renderer_->renderFrame();


            toFrameEnd = targetFrameTime - frameClock.getElapsedTime();
            if (toFrameEnd > sf::Time::Zero)
                sf::sleep(toFrameEnd);

            ++fpsCounter;

            lastFrameTime        = frameClock.getElapsedTime();
            GameTime::deltaTime_ = lastFrameTime.asMicroseconds() / 1000.;
            frameClock.restart();

            if (fpsClock.getElapsedTime() >= SECOND)
            {
                fpsClock.restart();
                std::cout << "FPS: " << fpsCounter << std::endl;
                lastFps_   = fpsCounter;
                fpsCounter = 0;
            }
        }
    }

    void Game::setUpGame()
    {
        window_ = {
           make_shared<sf::RenderWindow>(
               sf::VideoMode{WINDOW_WIDTH, WINDOW_HEIGHT},
               "MyGame",
               sf::Style::Fullscreen
           )
        };
        assetManager_ = {
            make_shared<AssetManager>("asset_info.json")
        };
        renderer_ = {
            make_shared<Renderer>(
                RENDERING_WIDTH,
                RENDERING_HEIGHT
            )
        };
        inputHandler_ = {
            make_shared<InputHandler>()
        };
        player_ = {
            make_shared<FPP_Player>(
                INITIAL_PLAYER_POS,
                INITIAL_PLAYER_DIR
            )
        };
        levelMap_= {
            make_shared<LevelMap>()
        };

        // shared_ptr<sf::RenderWindow> window(
        //     new sf::RenderWindow{
        //         sf::VideoMode{game::Game::WINDOW_WIDTH, game::Game::WINDOW_HEIGHT},
        //         "MyGame",
        //         sf::Style::Fullscreen,
        //     }
        // );
        // window_ = std::move(window);

        assetManager_->loadAssets();

        inputHandler_->setMouseLookSensitivityX(150);
        inputHandler_->setMovementSpeed(5);

        levelMap_->loadFromInts(levelMap_->TEST_MAP);

        // TEST ENTITIES
        levelMap_->entities().add(
            make_shared<Entity>(
                textureManager()->getTextureForName("frogmon_stand"),
                sf::Vector2d{15, 15}
            )
        );
        levelMap_->entities().add(
            make_shared<game::Entity>(
                textureManager()->getTextureForName("barrel"),
                sf::Vector2d{14, 8}
            )
        );
        shared_ptr<AnimatedEntity> animEnt{
            make_shared<AnimatedEntity>(
                textureManager()->getTextureForName("frogmon"),
                sf::Vector2d{20, 20}
            )
        };
        string animName = "frogmon_walk";
        Animation anim = animationManager()->getAnimationForName(animName);
        animEnt->addAnimation(animName, std::move(anim));
        animEnt->loopAnimation();
        levelMap_->entities().add(animEnt);

        std::this_thread::sleep_for(0.5s);

        animEnt = {
            make_shared<AnimatedEntity>(
                textureManager()->getTextureForName("frogmon"),
                sf::Vector2d{18, 20}
            )
        };
        animName = "frogmon_walk";
        anim = animationManager()->getAnimationForName(animName);
        animEnt->addAnimation(animName, std::move(anim));
        animEnt->loopAnimation();
        levelMap_->entities().add(animEnt);

    }
}