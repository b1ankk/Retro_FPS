#pragma once
#include "UserInterface.h"

namespace game
{
    class HealthUIE : public UserInterface::UI_Element
    {
    public:


        HealthUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition)
            : UI_Element(size, uiPosition)
        {
        }

        void createTexture(std::unique_ptr<sf::Texture>&) const override;
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    };

    class ArmorUIE : public UserInterface::UI_Element
    {
    public:
        ArmorUIE(const sf::Vector2i& size, const sf::Vector2i& uiPosition)
            : UI_Element(size, uiPosition)
        {
        }
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
