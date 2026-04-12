#ifndef PADDLE_H
#define PADDLE_H

#include <raylib.h>

class Paddle {
private:
    Vector2 position;
    float width;
    float height;
    float speed;
    int windowWidth;
public:
    void Init(Vector2 pos, float w, float h, float spd, int winWidth);
    void Update(float deltaTime);
    void Draw();
    Rectangle GetRect() const;
    Vector2 GetPosition() const;
};

#endif // PADDLE_H
