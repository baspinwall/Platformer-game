#ifndef Coin_h
#define Coin_h

#include "Item.h"

class Coin : public Item
{
    public:
        Coin(sf::Texture* texture, v2f position);
        ~Coin() = default;

        virtual void UpdatePlayer(Player &player) override;

    protected:


    private:
};

#endif /* Coin_hpp */
