#pragma once
#include <SFML/System/Vector2.hpp>

#include "vector_additions.h"

class RayCaster
{
public:
    double castRay()
    {
        
    }


private:
    sf::Vector2d& position_;
    sf::Vector2d& direction_;
    sf::Vector2d plane_;

    int width_;
    int height_;

};

