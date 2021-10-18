#include "Item.h"

Item::Item() { }

Item::Item(sf::Texture* texture, v2f position)
{
    body.setOrigin(v2f(32.0f, 32.0f));
    body.setTexture(*texture);
    body.setPosition(position);
}
