#pragma once
#include <SFML/Graphics/Text.hpp>

#include "UserInterface.h"

namespace game
{
    class HealthUIE : public UserInterface::UI_Element
    {
    public:


        HealthUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition);

        void createTexture(std::unique_ptr<sf::Texture>&) const override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update() override;

    private:
        sf::Text healthValue_;
        sf::Text healthText_;
    };


    class ArmorUIE : public UserInterface::UI_Element
    {
    public:
        ArmorUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition);

        void createTexture(std::unique_ptr<sf::Texture>&) const override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update() override;

    private:
        sf::Text armorValue_;
        sf::Text armorText_;
    };


    class AmmoUIE : public UserInterface::UI_Element
    {
    public:
        AmmoUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition);

        void createTexture(std::unique_ptr<sf::Texture>&) const override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        void update() override;

    private:
        sf::Text ammoValue_;
        sf::Text ammoText_;
    };


    class FillerUIE : public UserInterface::UI_Element
    {
    public:
        FillerUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition)
            : UI_Element(size, uiPosition)
        {
        }

        void createTexture(std::unique_ptr<sf::Texture>&) const override;
    };

}
