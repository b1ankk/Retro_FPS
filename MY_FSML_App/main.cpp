#include "ConfigLoader.h"
#include "Game.h"

int main(int argc, char** argv)
{
    game::ConfigLoader cl{"config.ini"};
    game::Config config = cl.load();
    
    game::Game::get().start();

}
