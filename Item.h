#ifndef Item_h
#define Item_h

#include "SFML/Graphics.hpp"
#include "Player.h"


class Item
{
    public:
        Item();
        Item(sf::Texture* texture, v2f position);
        ~Item() = default;

        sf::FloatRect GetGlobalBounds() { return body.getGlobalBounds(); }

        virtual void UpdatePlayer(Player &player) = 0;
        void Draw(sf::RenderWindow &window) { window.draw(body); }

    protected:
        sf::Sprite body;

    private:
};


#endif /* Item_hpp */
