#include "Key.h"

Key::Key(sf::Texture* texture, v2f position)
{
    body.setOrigin(v2f(32.0f, 32.0f));
    body.setTexture(*texture);
    body.setPosition(position);
}

/* virutal */
void Key::UpdatePlayer(Player &player)
{
    player.obtainedKey();
    body.setPosition(v2f(-1000.0f, 0.0f));
}
