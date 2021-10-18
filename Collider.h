#ifndef Collider_h
#define Collider_h

#include <SFML/Graphics.hpp>
#include <cmath>

using v2f = sf::Vector2f;

class Collider
{
    public:
        Collider(sf::Sprite &body);
        ~Collider() = default;

        v2f GetPosition() { return body.getPosition(); }
        v2f GetHalfSize() { return v2f(body.getGlobalBounds().width / 2.0f, body.getGlobalBounds().height / 2.0f); }

        void Move(float dx, float dy) { body.move(dx, dy); }
        bool CheckCollision(Collider other, v2f &direction, float push);


    protected:


    private:
        sf::Sprite &body;
};


#endif /* Collider_h */
