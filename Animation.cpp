#include "Animation.h"

Animation::Animation(sf::Texture* texture, v2u imageCount, float switchTime) : imageCount(imageCount), switchTime(switchTime)
{
    totalTime = 0.0f;
    currentImage.x = 0;

    uvRect.width = texture->getSize().x / static_cast<float>(imageCount.x);
    uvRect.height = texture->getSize().y / static_cast<float>(imageCount.y);

}

void Animation::Update(int row, float deltaTime, bool faceLeft)
{
    currentImage.y = row;
    totalTime += deltaTime;

    if(totalTime >= switchTime)
    {
        totalTime -= switchTime;
        currentImage.x++;

        if(currentImage.x >= imageCount.x)
        {
            currentImage.x = 0;
        }
    }

    uvRect.top = currentImage.y * uvRect.height;

    if(faceLeft)
    {
        uvRect.left = currentImage.x * uvRect.width;
        uvRect.width = abs(uvRect.width);
    } else
    {
        uvRect.left = (currentImage.x + 1) * abs(uvRect.width);
        uvRect.width = -abs(uvRect.width);
    }
}


