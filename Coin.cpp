#include "Coin.h"

Coin::Coin(sf::Texture* texture, v2f position) : Item(texture, position) { }

/* virutal */
void Coin::UpdatePlayer(Player &player)
{
    player.setScore(player.getScore() + 100);
    body.setPosition(v2f(-1000.0f, 0.0f));
}
