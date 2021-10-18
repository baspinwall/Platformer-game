#ifndef TextureLoadException_h
#define TextureLoadException_h

#include <iostream>
#include <string>

namespace Textures
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

#endif /* TextureLoadException_h */
