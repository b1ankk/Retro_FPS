#include <SFML/Graphics/RenderTarget.hpp>

#include "Entity.h"


namespace game
{
    Entity::Entity(std::shared_ptr<const sf::Texture> texture,
                   const sf::Vector2d&          mapPosition,
                   const sf::Vector2i&          imageSize,
                   sf::VertexArray              vertices):
        texture_(std::move(texture)),
        imageSize_(texture_->getSize().x, texture_->getSize().y),
        vertices_(std::move(vertices)),
        mapPosition_(mapPosition)
    {
        sf::Color color{0, 0, 0};

        // if no vertices passed create default square with imageSize as size
        if (vertices_.getVertexCount() == 0)
        {
            vertices_.append(
                sf::Vertex{
                    {0, 0},
                    sf::Color::White,
                    {0, 0}
                }
            );
            vertices_.append(
                sf::Vertex{
                    {static_cast<float>(imageSize_.x), 0},
                    sf::Color::White,
                    {static_cast<float>(imageSize_.x), 0}
                }
            );
            vertices_.append(
                sf::Vertex{
                    {static_cast<float>(imageSize_.x), static_cast<float>(imageSize_.y)},
                    sf::Color::White,
                    {static_cast<float>(imageSize_.x), static_cast<float>(imageSize_.y)}
                }
            );
            vertices_.append(
                sf::Vertex{
                    {0, static_cast<float>(imageSize_.y)},
                    sf::Color::White,
                    {0, static_cast<float>(imageSize_.y)}
                }
            );

            // set default origin as the middle of the entity
            sf::Transformable::setOrigin(imageSize_.x / 2, imageSize_.y / 2);

            
        }


    }

    void game::Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        // apply the entity's transform -- combine it with the one that was passed by the caller
        states.transform *= sf::Transformable::getTransform();

        states.texture = texture_.get();

        target.draw(vertices_, states);
    }


    
}
