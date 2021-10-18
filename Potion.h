#ifndef Potion_h
#define Potion_h

#include "Item.h"


class Potion : public Item
{
    public:
        Potion(sf::Texture* texture, v2f position);
        ~Potion() = default;

        virtual void UpdatePlayer(Player &player) override;

    protected:


    private:
        v2f pos;
};

#endif /* Potion_hpp */
