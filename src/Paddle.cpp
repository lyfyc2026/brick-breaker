#include "Paddle.h"
#include <raylib.h>

void Paddle::Init(Vector2 pos, float w, float h, float spd, int winWidth) {
    position = pos;
    width = w;
    height = h;
    speed = spd;
    windowWidth = winWidth;
}

void Paddle::Update(float deltaTime) {
    if (IsKeyDown(KEY_LEFT)) {
        position.x -= speed * deltaTime;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        position.x += speed * deltaTime;
    }

    if (position.x < 0) position.x = 0;
    if (position.x + width > windowWidth) {
        position.x = windowWidth - width;
    }
}

void Paddle::Draw() {
    DrawRectangleV(position, {width, height}, WHITE);
}

Rectangle Paddle::GetRect() const {
    return {position.x, position.y, width, height};
}

Vector2 Paddle::GetPosition() const {
    return position;
}
