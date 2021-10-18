#include "Trap.h"

Trap::Trap(sf::Texture* texture, v2f scale, v2f position, float rotation)
{
    body.setOrigin(v2f(50.0f, 50.0f));
    body.setTexture(*texture);
    body.setScale(scale);
    body.setPosition(position);
    body.setRotation(rotation);
}

void Trap::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}

void Trap::UpdatePlayer(Player &player)
{
    player.loseLives();
    player.TPCheckpoint();
}
