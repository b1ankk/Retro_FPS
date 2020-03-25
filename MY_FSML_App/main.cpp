#include <SFML/Graphics.hpp>
#include "LevelMap.h"
#include "AssetManager.h"
#include "Game.h"
#include "GameTime.h"
#include "InputHandler.h"
#include "Renderer.h"


using namespace std;

namespace game
{
    

    constexpr int MAP_WIDTH  = 24;
    constexpr int MAP_HEIGHT = 24;

    game::LevelMap tempWorldMap;

    
}

int main(int argc, char** argv)
{
    // CONSTANTS
    constexpr int WINDOW_WIDTH{1920};
    constexpr int WINDOW_HEIGHT{1080};
    const sf::Time SECOND{sf::seconds(1.f)};

    const sf::Vector2d INITIAL_PLAYER_POS{22, 12};
    const sf::Vector2d INITIAL_PLAYER_DIR{-1, 0};
 
    std::shared_ptr<sf::RenderWindow> window{
        new sf::RenderWindow{
            sf::VideoMode{WINDOW_WIDTH, WINDOW_HEIGHT},
            "MyGame"/*, sf::Style::Fullscreen*/
        }
    };

    // LOAD ASSETS

    game::AssetManager assetManager{"asset_info.json"};
    assetManager.loadAssets();


    // LOAD MAP

    game::tempWorldMap.setTileTypeManager(assetManager.tileTypeManager());
    
    
    // Player object
    std::shared_ptr<game::FPP_Player> player = std::make_shared<game::FPP_Player>(
        game::FPP_Player{
            INITIAL_PLAYER_POS,
            INITIAL_PLAYER_DIR
        }
    );
    

    // World renderer
    game::Renderer renderer{
        player,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        window,
        assetManager.spriteManager(),
    };

    // Player input handler
    game::InputHandler inputHandler{window, player};
    inputHandler.setMouseLookSensitivityX(100);
    inputHandler.setMovementSpeed(5);





    int fpsCounter{0};
    double targetFPS{144.};
    sf::Time targetFrameTime{
        sf::microseconds(static_cast<sf::Int64>(1'000'000. / targetFPS))
    };
    sf::Time lastFrameTime;
    game::GameTime::deltaTime_ = lastFrameTime.asMicroseconds() / 1000.f;
    

    sf::Clock fpsClock;
    sf::Clock frameClock;

    sf::Time toFrameEnd;
    while (window->isOpen())
    {
        // it is the most safe to handle window events in the function of creation
        sf::Event event; // NOLINT(cppcoreguidelines-pro-type-member-init, hicpp-member-init)
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    window->close();
                    exit(0);
                }
            }
        }
        inputHandler.handleInput();

        renderer.renderFrame();

        toFrameEnd = targetFrameTime - frameClock.getElapsedTime();
        if (toFrameEnd > sf::Time::Zero)
            sf::sleep(toFrameEnd);

        ++fpsCounter;

        lastFrameTime = frameClock.getElapsedTime();
        game::GameTime::deltaTime_ = lastFrameTime.asMicroseconds() / 1000.;
        frameClock.restart();

        if (fpsClock.getElapsedTime() >= SECOND)
        {
            fpsClock.restart();
            std::cout << "FPS: " << fpsCounter << std::endl;
            fpsCounter = 0;
        }
    }

}





