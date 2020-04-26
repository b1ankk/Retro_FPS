#include "Game.h"

#include <thread>
#include <vector>

#include "AnimatedEntity.h"
#include "Renderer.h"
#include "LevelMap.h"
#include "AssetManager.h"
#include "AnimationManager.h"
#include "Enemy.h"
#include "FPP_Player.h"
#include "InputHandler.h"
#include "TextureManager.h"
#include "GameTime.h"
#include "Gun.h"
#include "MapTile.h"
#include "CircleCollider.h"

namespace game
{
    using namespace std;
    using namespace chrono_literals;

    const sf::Vector2i Game::SCREEN_MIDDLE{WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2};
    const int          Game::RENDERING_WIDTH{WINDOW_WIDTH};
    const int          Game::RENDERING_HEIGHT{WINDOW_HEIGHT};

    const std::string Game::DEFAULT_FONT_NAME = "Pixeled";

    void Game::start()
    {
        initRandEngine();
        setUpGame();
        spawnRandomEnemies(128);

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
                        // exit(0);
                        return;
                    }
                    if (event.key.code == sf::Keyboard::F3)
                    {
                        renderer_->setDrawFpsCounter(!renderer_->ifDrawFpsCounter());
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed)
                {
                    if (event.mouseButton.button == sf::Mouse::Left)
                    {
                        player_->shoot();
                    }
                }
            }

            if (gameOver_)
            {
                renderer_->drawGameOverScreen();
                this_thread::sleep_for(1s);
                while (gameOver_)
                {
                    while (window_->pollEvent(event))
                    {
                        if (event.type == sf::Event::KeyPressed)
                        {
                            if (event.key.code == sf::Keyboard::Escape ||
                                event.key.code == sf::Keyboard::Enter ||
                                event.key.code == sf::Keyboard::Space)
                            {
                                window_->close();
                                return;
                            }
                        }
                    }
                    this_thread::sleep_for(100ms);
                }
            }


            inputHandler_->handleInput();

            levelMap_->update();

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
        assetManager_->loadAssets();

        renderer_ = {
            make_shared<Renderer>(
                RENDERING_WIDTH,
                RENDERING_HEIGHT
            )
        };
        renderer_->initUI();

        inputHandler_ = {
            make_shared<InputHandler>()
        };
        inputHandler_->setMouseLookSensitivityX(120);
        inputHandler_->setMovementSpeed(4);

        player_ = {
            make_shared<FPP_Player>(
                INITIAL_PLAYER_POS,
                INITIAL_PLAYER_DIR
            )
        };
        player_->rotate(90);
        std::shared_ptr<Gun> activeGun{
            make_shared<Gun>(
                "shotty",
                assetManager_->animationManager()->getAnimationForName("shotgun_shoot"),
                sf::seconds(0.8f),
                135.,
                3.5
            )
        };
        player_->setActiveGun(
            activeGun
        );

        levelMap_ = {
            make_shared<LevelMap>()
        };

        setUpEnemyPatterns();

        levelMap_->loadFromInts(levelMap_->TEST_MAP);
    }

    void Game::initRandEngine()
    {
        random_device rd;
        randEngine_ = mt19937(rd());
    }


    void Game::setUpEnemyPatterns()
    {
        // FROGMON 
        const string enemyName = "frogmon";
        Enemy        frogmon(
            textureManager()->getTextureForName(enemyName),
            sf::Vector2d{0, 0},
            100,
            24,
            1.3
        );
        Animation animation;

        string animName = "frogmon_idle";
        animation       = animationManager()->getAnimationForName(animName);
        frogmon.addAnimation(animName, std::move(animation));
        frogmon.setActiveAnimation(animName);

        animName  = "frogmon_walk";
        animation = animationManager()->getAnimationForName(animName);
        frogmon.addAnimation(animName, std::move(animation));

        animName  = "frogmon_die";
        animation = animationManager()->getAnimationForName(animName);
        animation.setMoveBackToFirstFrame(false);
        frogmon.addAnimation(animName, std::move(animation));

        animName  = "frogmon_bleed";
        animation = animationManager()->getAnimationForName(animName);
        frogmon.addAnimation(animName, std::move(animation));


        frogmon.setIdleAnimationName("frogmon_idle");
        frogmon.setDieAnimationName("frogmon_die");
        frogmon.setWalkAnimationName("frogmon_walk");
        frogmon.setBleedAnimationName("frogmon_bleed");

        frogmon.setCollider(
            make_shared<CircleCollider>(
                sf::Vector2d{0, 0},
                0.55
            )
        );

        enemyPatterns_.emplace(enemyName, std::move(frogmon));
        // Enemy::enemyPatterns_.insert({enemyName, std::move(frogmon)});
    }

    void Game::spawnRandomEnemies(int amount)
    {
        deque<sf::Vector2i> emptyPlaces;
        for (int x = 0; x < levelMap_->size().x; ++x)
            for (int y = 0; y < levelMap_->size().y; ++y)
                if (levelMap_->mapData()->at(x).at(y).isTraversable())
                    emptyPlaces.emplace_back(x, y);


        uniform_int_distribution<int>     randPosIndex{0, static_cast<int>(emptyPlaces.size()) - 1};
        uniform_real_distribution<double> randOffsetRangeX{0.3, 0.68};
        uniform_real_distribution<double> randOffsetRangeY{0.3, 0.68};


        while (levelMap_->enemies().size() < amount)
        {
            int posIndex = randPosIndex(randEngine_);

            int x = emptyPlaces[posIndex].x;
            int y = emptyPlaces[posIndex].y;

            double offsetX = randOffsetRangeX(randEngine_);
            double offsetY = randOffsetRangeY(randEngine_);

            double relativeX    = player_->position().x - x;
            double relativeY    = player_->position().y - y;
            double distToPlayer = sqrt(relativeX * relativeX + relativeY * relativeY);

            if (distToPlayer > 5 &&
                levelMap_->mapData()->at(x).at(y).isTraversable())
            {
                std::shared_ptr<Enemy> enemy{make_shared<Enemy>(enemyPatterns_.at("frogmon"))};
                enemy->setMapPosition(sf::Vector2d(x + offsetX, y + offsetY));
                levelMap_->addEnemy(enemy);
            }
        }
    }
}
