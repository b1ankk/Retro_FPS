#pragma once

#include <string>
#include <fstream>

#include "inipp/inipp.h"

namespace game
{
    struct Config;

    class ConfigLoader
    {
    public:
        ConfigLoader(const ConfigLoader&) = delete;

        explicit ConfigLoader(const std::string& configFileName)
        {
            this->fileName = configFileName;
        }


        const Config* load();

    private:
        std::string      fileName;
        std::ifstream    inFileStream;
        inipp::Ini<char> confIni;
        inipp::Ini<char> baseConfIni;

        bool newFileCreated{false};

        void open();
        void parse();

        template <typename T>
        T& readValue(const std::string& section, const std::string& name, T& destination)
        {
            T temp;
            if (inipp::extract(confIni.sections[section][name], temp))
                destination = std::move(temp);
            else if (inipp::extract(baseConfIni.sections[section][name], temp))
                destination = std::move(temp);
            else
                throw std::out_of_range{"defaut value not specified: " + name};

            return destination;
        }

        template <typename T>
        T& readDefaultValue(const std::string& section, const std::string& name, T& destination)
        {
            T temp;
            if (inipp::extract(baseConfIni.sections[section][name], temp))
                destination = std::move(temp);
            else
                throw std::out_of_range{"defaut value not specified: " + name};

            return destination;
        }


        const std::string SECTION_DEFAULT = "DEFAULT";
        const std::string SECTION_WINDOW = "Window";
        const std::string SECTION_INPUT = "Input";

        const std::string baseConfPattern = 1 + R"(
[Window]
; 0 - window, 1 - borderless window, 2 - fullscreen
openMode = 2 
width = 1920
height = 1080
maxFps = 0

[Input]
mouseSensitivityX = 1.0
)";

    };

    struct Config
    {
        const int wResWidth;
        const int wResHeight;
        const int wOpenMode;
        const int wMaxFps;
        const double iMouseSensitivityX;
    };

}