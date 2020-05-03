#include "ConfigLoader.h"

namespace game
{
    using namespace std;

    const Config* ConfigLoader::load()
    {
        open();
        parse();

        int wWidth;
        readValue(SECTION_WINDOW, "width", wWidth);

        int wHeight;
        readValue(SECTION_WINDOW, "height", wHeight);

        int wOpenMode;
        readValue(SECTION_WINDOW, "openMode", wOpenMode);
        if (wOpenMode < 0 || wOpenMode > 2)
            readDefaultValue(SECTION_WINDOW, "openMode", wOpenMode);

        int wMaxFps;
        readValue(SECTION_WINDOW, "maxFps", wMaxFps);
        if (wMaxFps < 0)
            readDefaultValue(SECTION_WINDOW, "maxFps", wMaxFps);
        


        const Config* config = new Config
        {
            wWidth,
            wHeight,
            wOpenMode,
            wMaxFps
        };

        return config;
    }

    void ConfigLoader::open()
    {
        inFileStream.open(fileName);
        if (!inFileStream.good())
        {
            ofstream out{fileName};
            out << baseConfPattern;
            out.close();
            inFileStream.open(fileName);
            newFileCreated = true;
        }
    }

    void ConfigLoader::parse()
    {
        stringstream tempBaseStream;
        tempBaseStream << baseConfPattern;
        baseConfIni.parse(tempBaseStream);

        if (!newFileCreated)
            confIni.parse(inFileStream);
        else
            confIni.parse(tempBaseStream);
    }
}
