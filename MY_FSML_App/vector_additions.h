#pragma once

#include <SFML/System/Vector2.hpp>

namespace sf
{
    typedef Vector2<double> Vector2d;
    

}

namespace game
{
    
    template <typename T>
    sf::Vector2<T> normalizeVector2(const sf::Vector2<T>& vector)
    {
        T vLength = sqrt(vector.x * vector.x + vector.y * vector.y);

        if (static_cast<long double>(vLength) == 0)
            return sf::Vector2<T>{};

        sf::Vector2<T> vNormalized{
            vector.x / vLength,
            vector.y / vLength
        };

        return vNormalized;
    }

}
