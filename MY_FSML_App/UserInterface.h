#pragma once
#include <cassert>
#include <memory>
#include <vector>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/Graphics/VertexArray.hpp>


namespace game
{
    class UserInterface : public sf::Drawable
    {
    public:
        UserInterface(const int&          width,
                      const int&          height,
                      const sf::Vector2i& drawingPosition);

        class UI_Element;


        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        // virtual void update() = 0;


        void addUI_Element(std::shared_ptr<UI_Element> element);
        void removeUI_Element(const size_t& index);

    private:
        int          width_;
        int          height_;
        sf::Vector2i drawingPosition_; // the point of origin

        std::vector<std::shared_ptr<UI_Element>> uiElements_;

    public:


        [[nodiscard]]
        int width() const
        {
            return width_;
        }

        [[nodiscard]]
        int height() const
        {
            return height_;
        }

        [[nodiscard]]
        sf::Vector2i drawingPosition() const
        {
            return drawingPosition_;
        }
    };

    ///////////////////    UI_Element class definition    ////////////////////////////////

    class UserInterface::UI_Element : public sf::Drawable, public sf::Transformable
    {
    public:
        UI_Element(const sf::Vector2i& size,
                   const sf::Vector2i& uiPosition) :
            vertices_(sf::PrimitiveType::Quads, 4),
            size_(size),
            uiPosition_(uiPosition)
        {
            vertices_[0] = {
                {0, 0},
                sf::Color::White,
                {0, 0}
            };
            vertices_[1] = {
                {static_cast<float>(size.x), 0},
                sf::Color::White,
                {static_cast<float>(size.x), 0}
            };
            vertices_[2] = {
                {static_cast<float>(size.x), static_cast<float>(size.y)},
                sf::Color::White,
                {static_cast<float>(size.x), static_cast<float>(size.y)}
            };
            vertices_[3] = {
                {0, static_cast<float>(size.y)},
                sf::Color::White,
                {0, static_cast<float>(size.y)}
            };

            // setOrigin(0, size.y);
        }

        // method needs to be called to initialize texture to draw
        virtual void createTexture(std::unique_ptr<sf::Texture>&) const = 0;


        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override
        {
            if (texture_ == nullptr)
                createTexture(texture_);

            assert(texture_ != nullptr && "texture_ must be initialized");

            states.transform *= sf::Transformable::getTransform();

            states.texture = texture_.get();

            target.draw(vertices_, states);
        }

        // virtual void update() = 0;


        sf::Vector2i size() const
        {
            return size_;
        }

        sf::Vector2i uiPosition() const
        {
            return uiPosition_;
        }

    protected:

        sf::VertexArray vertices_;

    private:
        mutable std::unique_ptr<sf::Texture> texture_{nullptr};

        sf::Vector2i size_;
        sf::Vector2i uiPosition_;
    };

    //////////////////////////    End of class definition ////////////////////////////
}
