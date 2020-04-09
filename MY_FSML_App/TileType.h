#pragma once
#include <memory>
#include <string>

namespace game
{
    class SpriteWrapper;


    class TileType
    {
    public:
        // TileType(const TileType&) = delete;
        ~TileType() = default;
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

    public:
        void setSprite(std::shared_ptr<const SpriteWrapper> spriteWrapper)
        {
            sprite_ = std::move(spriteWrapper);
        }


        [[nodiscard]]
        int id() const
        {
            return id_;
        }

        [[nodiscard]]
        std::string name() const
        {
            return name_;
        }

        [[nodiscard]]
        std::string spriteName() const
        {
            return spriteName_;
        }

        [[nodiscard]]
        bool isTraversable() const
        {
            return isTraversable_;
        }

        [[nodiscard]]
        std::shared_ptr<const game::SpriteWrapper> image() const
        {
            return sprite_;
        }
    };
}
