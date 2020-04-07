#pragma once

#include <memory>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>



#include "vector_additions.h"


namespace game
{
    class EntityVector;




    class Entity : public sf::Drawable, public sf::Transformable
    {
    public:
        Entity(std::shared_ptr<const sf::Texture> texture,
               const sf::Vector2d&                mapPosition,
               const sf::Vector2i&                imageSize = sf::Vector2i{64, 64}, // this is set to the texture's size in the constructor's body anyway
               sf::VertexArray                    vertices  = sf::VertexArray{sf::PrimitiveType::Quads});

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

        void moveOnMap(const sf::Vector2d& direction, const double& distance)
        {
            mapPosition_ += normalizeVector2(direction) * distance;
        }

        void resetVertices();

    private:
        friend EntityVector;

 
        std::shared_ptr<const sf::Texture> texture_;
        sf::Vector2i                       imageSize_;
        sf::VertexArray                    vertices_{sf::PrimitiveType::Quads};


        sf::Vector2d mapPosition_;

        double distanceToPlayer_{}; // distance to player used in sorting entities

    public:

        // GETTERS & SETTERS


        sf::VertexArray& vertices()
        {
            return vertices_;
        }

        sf::Vector2d mapPosition() const
        {
            return mapPosition_;
        }

        double distanceToPlayer() const
        {
            return distanceToPlayer_;
        }

        sf::Vector2i imageSize() const
        {
            return imageSize_;
        }
    };
}
