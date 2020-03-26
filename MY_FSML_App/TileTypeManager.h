#pragma once

#include <memory>
#include <SFML/Graphics/Sprite.hpp>


#include "SpriteManager.h"
#include "TileType.h"

namespace game
{
    class TileTypeManager
    {
    public:
        TileTypeManager()
        {
            TileType voidTileType{
                "void_tile",
                "none",
                true
            };
            voidTileType.setSprite(nullptr);

            std::shared_ptr<TileType> voidTileTypePtr{
                std::make_shared<TileType>(
                    std::move(voidTileType)
                )
            };

            tileTypeOrder_.push_back(voidTileTypePtr);
            tileTypesMap_.insert({"none", std::move(voidTileTypePtr)});
        }


        void loadTileTypeInfo(std::string, std::shared_ptr<game::TileType>);
        void loadAllTileTypes(const std::shared_ptr<const SpriteManager>&);

    private:

        std::unordered_map<std::string, std::shared_ptr<game::TileType>> tileTypesMap_{};
        std::vector<std::shared_ptr<game::TileType>>                     tileTypeOrder_;


    public:
        std::shared_ptr<TileType> getTypeForId(const size_t& id) const
        {
            if (id < tileTypeOrder_.size())
                return tileTypeOrder_[id];

            throw std::out_of_range{
                "No TileType found, for id: "
                + std::to_string(id)
            };
        }

        std::shared_ptr<TileType> getTypeForName(const std::string& name) const
        {
            try
            {
                return tileTypesMap_.at(name);
            }
            catch (const std::out_of_range&)
            {
                throw std::out_of_range("No TileType found, for name: " + name);
            }
        }
    };
}
