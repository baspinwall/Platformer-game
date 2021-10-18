#ifndef Trap_h
#define Trap_h

#include "SFML/Graphics.hpp"
#include "Player.h"


class Trap
{
    public:
        Trap(sf::Texture* texture, v2f scale, v2f position, float rotation);
        ~Trap()= default;

        sf::FloatRect GetGlobalBounds() { return body.getGlobalBounds(); }

        void Draw(sf::RenderWindow &window);
        void UpdatePlayer(Player &player);


    protected:


    private:
        sf::Sprite body;
};
#endif /* Trap_h */
