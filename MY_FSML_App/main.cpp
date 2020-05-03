#include "ConfigLoader.h"
#include "Game.h"

int main(int argc, char** argv)
{
    game::ConfigLoader cl{"config.ini"};

    game::Game::get().setConfig(cl.load());
    game::Game::get().start();

}
