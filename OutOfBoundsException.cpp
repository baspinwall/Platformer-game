#include "OutOfBoundsException.h"

namespace Bound
{
    std::ostream& operator<<(std::ostream& out, const Error& error)
    {
        out << "Player went out of bounds! Exiting..." << std::endl;
        return out;
    }
}
