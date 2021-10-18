#ifndef FontLoadException_h
#define FontLoadException_h

#include <iostream>
#include <string>

namespace Font
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
#endif /* FontLoadException_h */
