#ifndef BRICK_H
#define BRICK_H

#include <raylib.h>

class Brick {
private:
    Rectangle rect;
    bool isDestroyed;
public:
    Brick(Rectangle brickRect);
    Rectangle GetRect() const;
    void Destroy();
    bool IsActive() const;
    void Draw();
};

#endif // BRICK_H
