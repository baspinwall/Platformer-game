#ifndef Apple_h
#define Apple_h

#include "Item.h"


class Apple : public Item
{
    public:
        Apple(sf::Texture* texture, v2f position);
        ~Apple() = default;

        virtual void UpdatePlayer(Player &player) override;
        void Refresh() { body.setPosition(pos); }

    protected:


    private:
        v2f pos;
};
#endif /* Apple_h */
