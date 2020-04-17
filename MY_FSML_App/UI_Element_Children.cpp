#include "UI_Element_Children.h"


#include <memory>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Text.hpp>

#include "Game.h"
#include "UserInterface.h"
#include "TextureManager.h"
#include "FPP_Player.h"
#include "Animation.h"

namespace game
{
    using namespace std;

    static sf::Texture makeDefaultPane(const sf::Vector2i& size,
                                       const string&       leftTextureName,
                                       const string&       fillTextureName,
                                       const string&       rightTextureName)
    {
        sf::RenderTexture renderTexture;
        renderTexture.create(size.x, size.y);
        renderTexture.clear(sf::Color{0xffffff00}); // White alpha

        auto left  = Game::get().textureManager()->getTextureForName(leftTextureName);
        auto fill  = Game::get().textureManager()->getTextureForName(fillTextureName);
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

    void updateText(sf::Text&                  text,
                    UserInterface::UI_Element* thisPtr,
                    const string&              textString,
                    const float                fontSizeScaler,
                    const float                fontYPosScaler)
    {
        text.setString(textString);
        text.setCharacterSize(static_cast<unsigned>(thisPtr->size().y * fontSizeScaler * thisPtr->getScale().y));
        auto boundRectValue = text.getGlobalBounds();
        text.setOrigin(boundRectValue.width / 2, 0);
        text.setPosition(
            sf::Vector2f(
                thisPtr->getPosition() + sf::Vector2f(
                    thisPtr->size().x * thisPtr->getScale().x / 2.f,
                    thisPtr->size().y * thisPtr->getScale().y * fontYPosScaler
                )
            )
        );
    }

    ////////////////    HEALTH    ////////////////

    HealthUIE::HealthUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition)
        : UI_Element(size, uiPosition)
    {
        healthValue_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        healthValue_.setFillColor(sf::Color::Red);
        healthValue_.setOutlineColor(sf::Color::Black);
        healthValue_.setOutlineThickness(2);

        healthText_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        healthText_.setFillColor(sf::Color::White);
        healthText_.setOutlineColor(sf::Color::Black);
        healthText_.setOutlineThickness(1);
    }

    void HealthUIE::createTexture(unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(),
                "base_left",
                "base_strap",
                "base_divider"
            )
        );
    }


    void HealthUIE::update()
    {
        updateText(
            healthValue_,
            this,
            to_string(Game::get().player()->health()),
            1 / 3.f,
            0.25f
        );

        updateText(
            healthText_,
            this,
            "HEALTH",
            1 / 6.f,
            0.70f
        );
    }


    void HealthUIE::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        UserInterface::UI_Element::draw(target, states);

        target.draw(healthValue_);
        target.draw(healthText_);
    }




    //////////////////////    ARMOR    //////////////////////


    ArmorUIE::ArmorUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition) :
        UI_Element(size, uiPosition)
    {
        armorValue_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        armorValue_.setFillColor(sf::Color::Red);
        armorValue_.setOutlineColor(sf::Color::Black);
        armorValue_.setOutlineThickness(2);

        armorText_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        armorText_.setFillColor(sf::Color::White);
        armorText_.setOutlineColor(sf::Color::Black);
        armorText_.setOutlineThickness(1);
    }

    void ArmorUIE::createTexture(std::unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(),
                "base_strap",
                "base_strap",
                "base_right"
            )
        );
    }

    void ArmorUIE::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        UserInterface::UI_Element::draw(target, states);

        target.draw(armorValue_);
        target.draw(armorText_);
    }

    void ArmorUIE::update()
    {
        updateText(
            armorValue_,
            this,
            to_string(Game::get().player()->armor()),
            1 / 3.f,
            0.25f
        );

        updateText(
            armorText_,
            this,
            "ARMOR",
            1 / 6.f,
            0.70f
        );
    }



    //////////////////////    AMMO    /////////////////////////


    AmmoUIE::AmmoUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition) :
        UI_Element(size, uiPosition)
    {
        ammoValue_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        ammoValue_.setFillColor(sf::Color::Red);
        ammoValue_.setOutlineColor(sf::Color::Black);
        ammoValue_.setOutlineThickness(2);

        ammoText_.setFont(*Game::get().fontManager()->getFontForName(Game::DEFAULT_FONT_NAME));
        ammoText_.setFillColor(sf::Color::White);
        ammoText_.setOutlineColor(sf::Color::Black);
        ammoText_.setOutlineThickness(1);
    }

    void AmmoUIE::createTexture(std::unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(),
                "base_left",
                "base_strap",
                "base_right"
            )
        );
    }

    void AmmoUIE::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        UserInterface::UI_Element::draw(target, states);

        target.draw(ammoValue_);
        target.draw(ammoText_);
    }

    void AmmoUIE::update()
    {
        updateText(
            ammoValue_,
            this,
            "00/99",
            1 / 3.f,
            0.25f
        );

        updateText(
            ammoText_,
            this,
            "AMMO",
            1 / 6.f,
            0.70f
        );
    }



    //////////////////////    FILLER    //////////////////////////

    void FillerUIE::createTexture(unique_ptr<sf::Texture>& texturePtr) const
    {
        texturePtr = make_unique<sf::Texture>(
            makeDefaultPane(
                size(),
                "empty_left",
                "empty_strap",
                "empty_right"
            )
            );
    }



    ///////////////////    ANIMATED GUN    ////////////////////////////////


    AnimatedGunUIE::AnimatedGunUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition) :
        UI_Element(size, uiPosition)
    {
        setOrigin(size.x / 2.f, size.y);
        setPosition(sf::Vector2f(uiPosition.x, uiPosition.y));
    }

    void AnimatedGunUIE::createTexture(std::unique_ptr<sf::Texture>& texturePtr) const
    {
        
    }

    void AnimatedGunUIE::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        states.transform *= getTransform();
        states.texture = shootAnimation_.texture().get();
        target.draw(vertices_, states);
    }

    void AnimatedGunUIE::update()
    {
        
    }


    void AnimatedGunUIE::setShootAnimation(Animation animation)
    {
        shootAnimation_ = std::move(animation);
    }

    void AnimatedGunUIE::playShootAnimation()
    {
        shootAnimation_.play();
    }
}
