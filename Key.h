#ifndef Key_h
#define Key_h

#include "Item.h"


class Key : public Item
{
    public:
        Key(sf::Texture* texture, v2f position);
        ~Key() = default;

        virtual void UpdatePlayer(Player &player) override;

    protected:


    private:
};

#endif /* Key_h */
