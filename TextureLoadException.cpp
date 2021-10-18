#include "TextureLoadException.h"

namespace Textures
    {
        std::ostream& operator<<(std::ostream& out, const Error& error)
        {
            out << "Can't load texture! Exiting..." << std::endl;
            return out;
        }
    }
