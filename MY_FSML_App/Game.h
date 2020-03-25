#pragma once
#include <SFML/Graphics/RenderWindow.hpp>

#include "AssetManager.h"
#include "FPP_Player.h"
#include "LevelMap.h"

namespace game
{
    // using WindowPtr = std::shared_ptr<sf::RenderWindow>;
    // using PlayerPtr = std::shared_ptr<game::FPP_Player>;
    // using AssetManagerPtr = std::shared_ptr<game::AssetManager>;
    // using LevelMapPtr = std::shared_ptr<game::LevelMap>;

    typedef std::shared_ptr<sf::RenderWindow> WindowPtr;
    typedef std::shared_ptr<game::FPP_Player> PlayerPtr;
    typedef std::shared_ptr<game::AssetManager> AssetManagerPtr;
    typedef std::shared_ptr<game::LevelMap> LevelMapPtr;


    

    class Game
    {

    };
}


