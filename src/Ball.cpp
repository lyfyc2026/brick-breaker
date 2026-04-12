#include "Ball.h"
#include <raylib.h>

void Ball::Init(Vector2 pos, Vector2 spd, float rad, int winWidth, int winHeight) {
    position = pos;
    speed = spd;
    radius = rad;
    windowWidth = winWidth;
    windowHeight = winHeight;
}

void Ball::Update(float deltaTime) {
    position.x += speed.x * deltaTime;
    position.y += speed.y * deltaTime;

    if (position.x - radius < 0 || position.x + radius > windowWidth) {
        ReverseSpeedX();
    }
    if (position.y - radius < 0) {
        ReverseSpeedY();
    }
}

void Ball::Draw() {
    DrawCircleV(position, radius, WHITE);
}

bool Ball::CheckBrickCollision(const Brick& brick) {
    if (!brick.IsActive()) return false;
    return CheckCollisionCircleRec(position, radius, brick.GetRect());
}

bool Ball::CheckPaddleCollision(const Paddle& paddle) {
    return CheckCollisionCircleRec(position, radius, paddle.GetRect());
}

void Ball::ReverseSpeedX() {
    speed.x *= -1;
}

void Ball::ReverseSpeedY() {
    speed.y *= -1;
}

void Ball::Reset(Vector2 pos, Vector2 spd) {
    position = pos;
    speed = spd;
}

Vector2 Ball::GetPosition() const {
    return position;
}

Vector2 Ball::GetSpeed() const {
    return speed;
}

float Ball::GetRadius() const {
    return radius;
}
