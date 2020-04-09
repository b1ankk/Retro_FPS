#include <SFML/Graphics/RenderTarget.hpp>
#include <utility>

#include "Entity.h"


namespace game
{
    using namespace std;

    // TODO make imageSize argument actually work
    Entity::Entity(shared_ptr<const sf::Texture> texture,
                   const sf::Vector2d&                mapPosition,
                   const sf::Vector2i&                imageSize):
        texture_(std::move(texture)),
        imageSize_(texture_->getSize().x, texture_->getSize().y),
        // vertices_(sf::PrimitiveType::Quads),
        mapPosition_(mapPosition)
    {
        // sf::Color color{0, 0, 0};

        // if no vertices passed create default square with imageSize as size
        if (vertices_.getVertexCount() == 0)
        {
            vertices_.resize(4);
            resetVertices();

            // set default origin as the middle of the entity
            sf::Transformable::setOrigin(imageSize_.x / 2.f, imageSize_.y / 2.f);
        }
    }

    // Entity::Entity(shared_ptr<const sf::Texture> texture,
    //                const sf::Vector2d&           mapPosition,
    //                const sf::Vector2i&           imageSize) :
    //     Entity(std::move(texture),
    //            mapPosition,
    //            imageSize,
    //            sf::VertexArray{sf::PrimitiveType::Quads})
    // {
    // }

    void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= sf::Transformable::getTransform();

        states.texture = texture_.get();

        target.draw(vertices_, states);
    }

    void Entity::resetVertices()
    {
        vertices_[0] = (
            sf::Vertex{
                {0, 0},
                sf::Color::White,
                {0, 0}
            }
        );
        vertices_[1] = (
            sf::Vertex{
                {static_cast<float>(imageSize_.x), 0},
                sf::Color::White,
                {static_cast<float>(imageSize_.x), 0}
            }
        );
        vertices_[2] = (
            sf::Vertex{
                {static_cast<float>(imageSize_.x), static_cast<float>(imageSize_.y)},
                sf::Color::White,
                {static_cast<float>(imageSize_.x), static_cast<float>(imageSize_.y)}
            }
        );
        vertices_[3] = (
            sf::Vertex{
                {0, static_cast<float>(imageSize_.y)},
                sf::Color::White,
                {0, static_cast<float>(imageSize_.y)}
            }
        );
    }
}
