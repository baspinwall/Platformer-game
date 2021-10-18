#ifndef Player_h
#define Player_h

#include "Animation.h"
#include "Collider.h"

using v2u = sf::Vector2u;
using v2f = sf::Vector2f;

class Player
{
    public:
        Player(sf::Texture* texture, v2u imageCount, float switchTime, float speed, float jumpHeight);
        ~Player() = default;

        v2f GetPosition() { return body.getPosition(); }
        v2f GetOrigin() { return body.getOrigin(); }
        sf::FloatRect GetGlobalBounds() { return body.getGlobalBounds(); }
        int getScore() { return score; }
        int getLives() { return lives; }
        bool getKeyStatus() { return hasKey; }
        bool getWinStatus() { return hasWon; }
        bool getLoseStatus() { return hasLost; }
        bool getFinishStatus() { return hasFinished; }
        v2f getCheckpoint() { return checkpoint; }
        bool getAtHighscore() { return atHighscore; }
        Collider GetCollider() { return Collider(body); }

        void setScore(int s) { score = s; }
        void setWin(bool) { hasWon = true; }
        void setLose(bool) { hasLost = true; }
        void setFinished(bool) { hasFinished = true; }
        void setCheckpoint(v2f cp) { checkpoint = cp; }
        void obtainedKey() { hasKey = true; }

        void Update(float deltaTime);
        void Draw(sf::RenderWindow &window);
        void OnCollision(v2f direction);
        void addLives() { lives++; }
        void loseLives() { lives--; }

        void TP(v2f dest) { body.setPosition(dest); }
        void TPPart1() { body.setPosition(v2f(0.0f, 0.0f)); }
        void TPPart2() { body.setPosition(v2f(-50.0f, 3150.0f)); }
        void TPPart3() { body.setPosition(v2f(7600.0f, -850.0f)); }
        void TPEnd() { body.setPosition(v2f(11500.0f, 0.0f)); }
        void TPHighscore() { body.setPosition(v2f(15000.0f, 0.0f)); atHighscore = true; }
        void TPCheckpoint() { body.setPosition(v2f(checkpoint.x, checkpoint.y - 100)); }

        void Boost() { speed = 600; jumpHeight = 300; }
        void Revert() { speed = 300; jumpHeight = 150; }


    protected:


    private:
        sf::Sprite body;

        Animation animation;
        unsigned row{0};
        bool faceLeft{false};

        unsigned score{0};
        unsigned lives{5};
        float speed;
        v2f velocity;
        unsigned wallJumpWindowL{0};
        unsigned wallJumpWindowR{0};
        bool canJump;
        float jumpHeight;

        bool hasKey{false};
        bool hasWon{false};
        bool hasLost{false};
        bool hasFinished{false};
        bool atHighscore{false};
        v2f checkpoint{v2f(0.0f, 0.0f)};
};

#endif /* Player_hpp */
