#pragma once
#include <memory>
#include <vector>

#include "vector_additions.h"

namespace game
{
    class Entity;
    class LevelMap;


    class EntityVector
    {
    public:


        void add(std::shared_ptr<game::Entity> entity);
        void remove(const size_t& index);



        void updateToPos(const sf::Vector2d& player)
        {
            updateDistancesTo(player);
            sortEntities();
        }

        auto begin()
        {
            return entities_.begin();
        }

        auto end()
        {
            return entities_.end();
        }

    private:
        std::vector<std::shared_ptr<game::Entity>> entities_{};


        // METHODS

        void updateDistancesTo(const sf::Vector2d&); // calculates and updates distances to a position (most likely player's) that are used in sorting

        void sortEntities();

    };

    
}
