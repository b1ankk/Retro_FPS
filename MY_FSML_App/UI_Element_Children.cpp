#include "UI_Element_Children.h"


#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>

#include "Game.h"
#include "UserInterface.h"
#include  "TextureManager.h"

namespace game
{
    using namespace std;

    static sf::Texture makeDefaultPane(const sf::Vector2i& size, 
                                       const string& leftTextureName,
                                       const string& fillTextureName,
                                       const string& rightTextureName)
    {
        sf::RenderTexture renderTexture;
        renderTexture.create(size.x, size.y);
        renderTexture.clear(sf::Color{0xffffff00}); // White alpha

        auto left = Game::get().textureManager()->getTextureForName(leftTextureName);
        auto fill = Game::get().textureManager()->getTextureForName(fillTextureName);
        auto right = Game::get().textureManager()->getTextureForName(rightTextureName);

        assert(unsigned(size.x) >= left->getSize().x + right->getSize().x);

        sf::RectangleShape leftRect{static_cast<sf::Vector2f>(left->getSize())};
        sf::RectangleShape rightRect{static_cast<sf::Vector2f>(right->getSize())};
        sf::RectangleShape fillRect{
            sf::Vector2f{
                static_cast<float>(size.x) - (rightRect.getSize().x + leftRect.getSize().x),
                static_cast<float>(fill->getSize().y)
            }
        };

        const_cast<sf::Texture*>(fill.get())->setRepeated(true);
        const_cast<sf::Texture*>(fill.get())->setSmooth(true);

        fillRect.setPosition(leftRect.getSize().x, fillRect.getPosition().y);
        rightRect.setPosition(size.x - rightRect.getSize().x, rightRect.getPosition().y);

        leftRect.setTexture(left.get());
        fillRect.setTexture(fill.get());
        fillRect.setTextureRect(
            sf::IntRect{
                {0, 0},
                static_cast<sf::Vector2i>(fillRect.getSize())
            }
        );
        rightRect.setTexture(right.get());

        renderTexture.draw(leftRect);
        renderTexture.draw(fillRect);
        renderTexture.draw(rightRect);
        renderTexture.display();

        return renderTexture.getTexture();
    }


    void HealthUIE::createTexture(unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(), "base_left", "base_strap", "base_right"
            )
        );
    }


    void HealthUIE::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        UserInterface::UI_Element::draw(target, states);


    }

    void FillerUIE::createTexture(unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(), "empty_left", "empty_strap", "empty_right"
            )
        );
    }
}
