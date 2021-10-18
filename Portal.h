#ifndef Portal_h
#define Portal_h

#include "SFML/Graphics.hpp"
#include "Player.h"


class Portal
{
    public:
        Portal(sf::Texture* texture, v2f scale, v2f position, v2f dest);
        ~Portal() = default;

        sf::FloatRect GetGlobalBounds() { return body.getGlobalBounds(); }

        void Draw(sf::RenderWindow &window) { window.draw(body); }
        void UpdatePlayer(Player &player);


    protected:
        sf::Sprite body;


    private:
        v2f destination;
        v2f checkpoint;
};
#endif /* Portal_hpp */
