#include "Animation.h"

#include <cassert>


namespace game
{
    void Animation::play()
    {
        if (!playing_)
        {
            clock_.restart();
            playing_ = true;
        }
    }

    void Animation::loop()
    {
        repeating_ = true;
        play();
    }


    void Animation::pause()
    {
        pausedTime_ += clock_.getElapsedTime();
        playing_ = false;
    }

    void Animation::stop()
    {
        playing_ = false;
        repeating_ = false;
        currentFrame_ = 0;
        pausedTime_ = sf::Time::Zero;
    }

    void Animation::stopRepeating()
    {
        repeating_ = false;
    }



    std::shared_ptr<const sf::Texture> Animation::texture()
    {
        assert(!frames_.empty() && "Cannot return the texture from an empty animation");

        // if not playing animation, return last visible frame
        if (!playing_)
            return frames_[currentFrame_].texture();

        // if time to change frame
        if (clock_.getElapsedTime() + pausedTime_ >= frames_[currentFrame_].endTime())
        {
            // if last frame of animation, come back to beginning
            if (currentFrame_ == frames_.size() - 1)
            {
                currentFrame_ = 0;
                pausedTime_ = sf::Time::Zero;
                // if not repeating, stop playing 
                if (!repeating_)
                    playing_ = false;
                // if still playing, restart animation clock
                else
                    clock_.restart();
            }
            // if some other frame, move to the next one
            else
            {
                ++currentFrame_;
            }
        }

        return frames_[currentFrame_].texture();
    }
}
