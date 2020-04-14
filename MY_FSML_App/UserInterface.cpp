#include "UserInterface.h"

#include <memory>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>

namespace game
{
    using namespace std;



    UserInterface::UserInterface(const int&   width,
                                 const int&   height,
                                 const sf::Vector2i& drawingPosition) :
        width_(width),
        height_(height),
        drawingPosition_(drawingPosition)
    {
    }


    void UserInterface::draw(sf::RenderTarget& target, sf::RenderStates states) const
    {
        for (auto& element : uiElements_)
        {
            element->draw(target, states);
        }
    }

    void UserInterface::addUI_Element(shared_ptr<UI_Element> element)
    {
        element->setPosition(static_cast<sf::Vector2f>(drawingPosition_ + element->uiPosition()));
        uiElements_.push_back(std::move(element));
    }

    void UserInterface::removeUI_Element(const size_t& index)
    {
        uiElements_.erase(uiElements_.begin() + index);
    }
}
