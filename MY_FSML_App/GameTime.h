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

        /**
         * \brief The time in milliseconds it took to generate the last frame.
         */
        static double deltaTime()
        {
            return deltaTime_;
        }

    private:
        // GameTime() = default;

        static double deltaTime_;

    };

    

}

