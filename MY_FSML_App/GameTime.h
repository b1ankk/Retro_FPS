#pragma once

int main(int argc, char** argv);

namespace game
{
    class GameTime
    {
    public:
        friend int ::main(int argc, char** argv);

        static double deltaTime()
        {
            return deltaTime_;
        }

    private:
        GameTime() = default;

        static double deltaTime_;

    };

    

}

