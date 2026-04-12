#include "Brick.h"

Brick::Brick(Rectangle brickRect) {
    rect = brickRect;
    isDestroyed = false;
}

Rectangle Brick::GetRect() const {
    return rect;
}

void Brick::Destroy() {
    isDestroyed = true;
}

bool Brick::IsActive() const {
    return !isDestroyed;
}

void Brick::Draw() {
    if (IsActive()) {
        DrawRectangleRec(rect, BLUE);
        DrawRectangleLinesEx(rect, 2, WHITE);
    }
}

