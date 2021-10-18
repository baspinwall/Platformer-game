#ifndef Platform_h
#define Platform_h

#include "SFML/Graphics.hpp"
#include "Collider.h"


class Platform
{
    public:
        Platform(sf::Texture* texture, v2f scale, v2f position);
        ~Platform() = default;

        Collider GetCollider() { return Collider(body); }

        void Draw(sf::RenderWindow &window);


    protected:


    private:
        sf::Sprite body;
};

#endif /* Platform_h */
