#pragma once
#include <SFML/System/Vector2.hpp>


namespace sf
{
    typedef Vector2<double> Vector2d;
    

}

namespace game
{
    template <typename T>
    T calcDotProduct(sf::Vector2<T> v1, sf::Vector2<T> v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template <typename T>
    T calcVectorLength(sf::Vector2<T> vector)
    {
        return sqrt(vector.x * vector.x + vector.y * vector.y);
    }

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
