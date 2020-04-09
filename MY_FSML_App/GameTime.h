#pragma once

int main(int argc, char** argv);

namespace game
{
    class Game;

    class GameTime
    {
    public:
        GameTime() = delete;
        GameTime(const GameTime&) = delete;
        ~GameTime() = default;

        friend Game;

        static double deltaTime()
        {
            return deltaTime_;
        }

    private:
        // GameTime() = default;

        static double deltaTime_;

    };

    

}

