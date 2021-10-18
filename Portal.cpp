#include "Portal.h"

Portal::Portal(sf::Texture* texture, v2f scale, v2f position, v2f dest)
    : destination(dest), checkpoint(dest)
{
    body.setOrigin(v2f(50.0f, 50.0f));
    body.setTexture(*texture);
    body.setScale(scale);
    body.setPosition(position);
}

void Portal::UpdatePlayer(Player &player)
{
    player.TP(destination);
    player.setCheckpoint(checkpoint);
}
