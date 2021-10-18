#ifndef SoundLoadException_h
#define SoundLoadException_h

#include <iostream>
#include <string>

namespace Sound
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
#endif /* SoundLoadException_h */
