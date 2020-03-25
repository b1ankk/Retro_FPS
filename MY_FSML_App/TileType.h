#pragma once
#include <memory>
#include <string>
#include <utility>
#include <vector>

#include "SpriteWrapper.h"

namespace game
{
    class TileType
    {
    public:

        //TODO TileType
        explicit TileType(std::string name,
                          std::string spriteName,
                          const bool isTraversable = false, 
                          const int id = nextId_++)
            : id_(id),
              name_(std::move(name)),
              spriteName_(std::move(spriteName)),
              isTraversable_(isTraversable)
        {
        }

        
        


    private:
        const int id_;
        const std::string name_;
        std::string spriteName_;
        std::shared_ptr<const SpriteWrapper> sprite_{};
        bool isTraversable_{};

        static int nextId_;


        static std::vector<std::shared_ptr<TileType>> tileTypes_;
    

    public:
        void setSprite(std::shared_ptr<const SpriteWrapper> spriteWrapper)
        {
            sprite_ = std::move(spriteWrapper);
        }


        int id() const
        {
            return id_;
        }

        std::string name() const
        {
            return name_;
        }

        std::string spriteName() const
        {
            return spriteName_;
        }

        bool isTraversable() const
        {
            return isTraversable_;
        }
    };
}
