#include "Apple.h"

Apple::Apple(sf::Texture* texture, v2f position) : Item(texture, position), pos(position) { }

/* virutal */
void Apple::UpdatePlayer(Player &player)
{
    player.Boost();
    body.setPosition(v2f(-1000.0f, 0.0f));
}
