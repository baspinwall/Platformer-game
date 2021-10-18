#ifndef Animation_h
#define Animation_h

#include "SFML/Graphics.hpp"

using v2u = sf::Vector2u;

class Animation
{
    public:
        Animation(sf::Texture* texture, v2u imageCount, float switchTime);
        ~Animation() = default;

        sf::IntRect uvRect;

        void Update(int row, float deltaTime, bool faceLeft);


    protected:


    private:
        v2u imageCount;
        v2u currentImage;

        float totalTime;
        float switchTime;
};

#endif /* Animation_h */
