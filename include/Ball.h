#ifndef BALL_H
#define BALL_H

#include <raylib.h>
#include "Brick.h"
#include "Paddle.h"

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    float radius;
    int windowWidth;
    int windowHeight;
public:
    void Init(Vector2 pos, Vector2 spd, float rad, int winWidth, int winHeight);
    void Update(float deltaTime);
    void Draw();
    bool CheckBrickCollision(const Brick& brick);
    bool CheckPaddleCollision(const Paddle& paddle);
    void ReverseSpeedX();
    void ReverseSpeedY();
    void Reset(Vector2 pos, Vector2 spd);
    Vector2 GetPosition() const;
    Vector2 GetSpeed() const;
    float GetRadius() const;
};

#endif // BALL_H
