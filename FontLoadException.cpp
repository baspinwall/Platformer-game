#include "FontLoadException.h"

namespace Font
{
    std::ostream& operator<<(std::ostream& out, const Error& error)
    {
        out << "Can't load font! Exiting..." << std::endl;
        return out;
    }
}
