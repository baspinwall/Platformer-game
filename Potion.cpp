#include "Potion.h"

Potion::Potion(sf::Texture* texture, v2f position) : Item(texture, position), pos(position) { }

/* virutal */
void Potion::UpdatePlayer(Player &player)
{
    player.addLives();
    body.setPosition(v2f(-1000.0f, 0.0f));
}
