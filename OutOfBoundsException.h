#ifndef OutOfBoundsException_h
#define OutOfBoundsException_h

#include <iostream>
#include <string>

namespace Bound
{
    class Error
    {
        private:
            std::string message;
        public:
            Error(std::string text) : message(text) { }
            friend std::ostream& operator<<(std::ostream& out, const Error& error);
    };
}

#endif /* OutOfBoundsException_h */
