#include "SoundLoadException.h"

namespace Sound
{
    std::ostream& operator<<(std::ostream& out, const Error& error)
    {
        out << "Can't load sound! Exiting..." << std::endl;
        return out;
    }
}

