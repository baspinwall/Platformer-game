#include "Player.h"

#include <iostream>
#include <cmath>

Player::Player(sf::Texture* texture, v2u imageCount, float switchTime, float speed, float jumpHeight)
    : animation(texture, imageCount, switchTime)
{
    this->speed = speed;
    this->jumpHeight = jumpHeight;

    body.setTexture(*texture);
    body.setOrigin(v2f(48.0f, 48.0f));
    body.setPosition(v2f(0.0f, 0.0f));
    body.setScale(v2f(2.083f, 2.083f));
}

void Player::Update(float deltaTime)
{
    velocity.x = 0.0f;

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        velocity.x -= speed;
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        velocity.x += speed;
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canJump)
    {
        canJump = false;
        velocity.y = -sqrtf(2.0f * 981.0f * jumpHeight);
    }

    velocity.y += 981.0f * deltaTime;


    if(velocity.x == 0.0f)
    {
        row = 0;
    } else
    {
        row = 1;

        if(velocity.x < 0.0f)
        {
            faceLeft = true;
        } else
        {
            faceLeft = false;
        }
    }

    if(!canJump)
    {
        row = 2;
    }

    if(hasWon && !atHighscore)
    {
        body.setPosition(v2f(13500.0f, 0.0f));
        row = 3;
        velocity.x = velocity.y = 0;
        faceLeft = true;
    }

    if(hasLost && !atHighscore)
    {
        body.setPosition(v2f(13500.0f, 0.0f));
        row = 4;
        velocity.x = velocity.y = 0;
    }

    if(atHighscore)
    {
        velocity.x = velocity.y = 0;
        row = 5;
        faceLeft = true;
    }

    animation.Update(row, deltaTime, faceLeft);
    body.setTextureRect(animation.uvRect);
    body.move(velocity * deltaTime);
}

void Player::Draw(sf::RenderWindow &window)
{
    window.draw(body);
}

void Player::OnCollision(v2f direction)
{
    if(direction.x < 0.0f)
    {
        //Collision on left
        velocity.x = 0.0f;
        wallJumpWindowL++;
        if(wallJumpWindowL < 100)
            canJump = true;
    } else if(direction.x > 0.0f)
    {
        //Collision on right
        velocity.x = 0.0f;
        wallJumpWindowR++;
        if(wallJumpWindowR < 100)
            canJump = true;
    }

    if(direction.y < 0.0f)
    {
        //Collision on bottom
        wallJumpWindowL = 0;
        wallJumpWindowR = 0;
        velocity.y = 0.0f;
        canJump = true;
    } else if(direction.y > 0.0f)
    {
        //Collision on top
        velocity.y = 0.0f;
    }
}
