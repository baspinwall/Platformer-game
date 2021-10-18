#include "Platform.h"

Platform::Platform(sf::Texture* texture, v2f scale, v2f position)
{
    body.setOrigin(v2f(50.0f, 50.0f));
    body.setScale(scale);
    body.setTexture(*texture);
    body.setPosition(position);
}

void Platform::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}
