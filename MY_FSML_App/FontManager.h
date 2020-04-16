#pragma once
#include <memory>
#include <SFML/Graphics/Font.hpp>
#include <string>
#include <unordered_map>
#include <deque>


namespace game
{
    class FontManager
    {
    public:

    private:
        std::unordered_map<std::string, std::shared_ptr<sf::Font>> fonts_;
        std::deque<std::shared_ptr<sf::Font>> fontsId_;


    public:
        void addFont(std::string name, std::shared_ptr<sf::Font> font)
        {
            fontsId_.emplace_back(font);
            fonts_.insert({std::move(name), std::move(font)});
        }



        std::shared_ptr<sf::Font> getFontForName(const std::string& name)
        {
            return fonts_.at(name);
        }


        std::shared_ptr<sf::Font> getFontForId(size_t index)
        {
            return fontsId_[index];
        }


    };
}
