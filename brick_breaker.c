#include "raylib.h"
#include <stdbool.h>
#include <math.h>

// 游戏窗口设置
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 600
#define FPS           60

// 玩家板属性
#define PADDLE_WIDTH  120
#define PADDLE_HEIGHT 15
#define PADDLE_SPEED  8

// 球属性
#define BALL_RADIUS   10
#define BALL_SPEED_X  5
#define BALL_SPEED_Y  -5

// 砖块属性
#define BRICK_ROWS    5
#define BRICK_COLS    10
#define BRICK_WIDTH   75
#define BRICK_HEIGHT  25
#define BRICK_GAP     5
#define BRICK_OFFSET_TOP  50
#define BRICK_OFFSET_LEFT 12

// 游戏全局数据
typedef struct {
    Rectangle paddle;
    Vector2 ballPos;
    Vector2 ballSpeed;
    Rectangle bricks[BRICK_ROWS * BRICK_COLS];
    int brickCount;
    int activeBrickCount;
    int score;
    int lives;
    bool gameOver;
    bool gameWin;
} Game;

Game game;

// -------------------------- 核心碰撞检测函数 --------------------------
// 1. 球与墙壁碰撞
void CheckWallCollision(void) {
    if (game.ballPos.x - BALL_RADIUS <= 0 || game.ballPos.x + BALL_RADIUS >= SCREEN_WIDTH) {
        game.ballSpeed.x *= -1;
    }
    if (game.ballPos.y - BALL_RADIUS <= 0) {
        game.ballSpeed.y *= -1;
    }
}

// 2. 球与玩家板碰撞
void CheckPaddleCollision(void) {
    if (CheckCollisionCircleRec(game.ballPos, BALL_RADIUS, game.paddle)) {
        game.ballSpeed.y *= -1;
        float hitOffset = (game.ballPos.x - game.paddle.x - game.paddle.width/2) / (game.paddle.width/2);
        game.ballSpeed.x = hitOffset * 6;
    }
}

// 3. 球与砖块碰撞（修复版：一次只消除一个砖块）
void CheckBrickCollision(void) {
    bool hasCollided = false; // 一帧只处理一次碰撞

    for (int i = 0; i < game.brickCount; i++) {
        if (game.bricks[i].width <= 0 || hasCollided) continue;

        if (CheckCollisionCircleRec(game.ballPos, BALL_RADIUS, game.bricks[i])) {
            hasCollided = true;

            // 击碎砖块
            game.bricks[i].width = 0;
            game.activeBrickCount--;
            game.score += 10;

            // 计算碰撞方向与重叠量
            float brickCenterX = game.bricks[i].x + BRICK_WIDTH/2;
            float brickCenterY = game.bricks[i].y + BRICK_HEIGHT/2;
            float dx = game.ballPos.x - brickCenterX;
            float dy = game.ballPos.y - brickCenterY;
            float overlapX = BALL_RADIUS + BRICK_WIDTH/2 - fabs(dx);
            float overlapY = BALL_RADIUS + BRICK_HEIGHT/2 - fabs(dy);

            // 分离球和砖块，避免二次碰撞
            if (overlapX < overlapY) {
                game.ballSpeed.x *= -1;
                game.ballPos.x += (dx > 0) ? overlapX : -overlapX;
            } else {
                game.ballSpeed.y *= -1;
                game.ballPos.y += (dy > 0) ? overlapY : -overlapY;
            }

            // 处理完一个砖块立刻退出
            break;
        }
    }
}

// -------------------------- 游戏初始化 --------------------------
void InitGame(void) {
    game.paddle = (Rectangle){
        (SCREEN_WIDTH - PADDLE_WIDTH) / 2,
        SCREEN_HEIGHT - 40,
        PADDLE_WIDTH, PADDLE_HEIGHT
    };

    game.ballPos = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60};
    game.ballSpeed = (Vector2){BALL_SPEED_X, BALL_SPEED_Y};

    game.brickCount = BRICK_ROWS * BRICK_COLS;
    game.activeBrickCount = game.brickCount;
    int index = 0;
    for (int row = 0; row < BRICK_ROWS; row++) {
        for (int col = 0; col < BRICK_COLS; col++) {
            game.bricks[index] = (Rectangle){
                BRICK_OFFSET_LEFT + col * (BRICK_WIDTH + BRICK_GAP),
                BRICK_OFFSET_TOP + row * (BRICK_HEIGHT + BRICK_GAP),
                BRICK_WIDTH, BRICK_HEIGHT
            };
            index++;
        }
    }

    game.score = 0;
    game.lives = 3;
    game.gameOver = false;
    game.gameWin = false;
}

// -------------------------- 玩家输入处理 --------------------------
void HandleInput(void) {
    if (game.gameOver || game.gameWin) {
        if (IsKeyPressed(KEY_R)) InitGame();
        return;
    }

    if (IsKeyDown(KEY_LEFT) && game.paddle.x > 0) {
        game.paddle.x -= PADDLE_SPEED;
    }
    if (IsKeyDown(KEY_RIGHT) && game.paddle.x + game.paddle.width < SCREEN_WIDTH) {
        game.paddle.x += PADDLE_SPEED;
    }
}

// -------------------------- 游戏逻辑更新 --------------------------
void UpdateGame(void) {
    if (game.gameOver || game.gameWin) return;

    game.ballPos.x += game.ballSpeed.x;
    game.ballPos.y += game.ballSpeed.y;

    // 限制球的最大速度，避免穿模
    #define MAX_BALL_SPEED 8.0f
    if (fabs(game.ballSpeed.x) > MAX_BALL_SPEED) {
        game.ballSpeed.x = (game.ballSpeed.x > 0) ? MAX_BALL_SPEED : -MAX_BALL_SPEED;
    }
    if (fabs(game.ballSpeed.y) > MAX_BALL_SPEED) {
        game.ballSpeed.y = (game.ballSpeed.y > 0) ? MAX_BALL_SPEED : -MAX_BALL_SPEED;
    }

    CheckWallCollision();
    CheckPaddleCollision();
    CheckBrickCollision();

    if (game.ballPos.y - BALL_RADIUS > SCREEN_HEIGHT) {
        game.lives--;
        if (game.lives <= 0) {
            game.gameOver = true;
        } else {
            game.ballPos = (Vector2){SCREEN_WIDTH / 2, SCREEN_HEIGHT - 60};
            game.ballSpeed = (Vector2){BALL_SPEED_X, BALL_SPEED_Y};
        }
    }

    if (game.activeBrickCount <= 0) {
        game.gameWin = true;
    }
}

// -------------------------- 游戏画面绘制 --------------------------
void DrawGame(void) {
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRec(game.paddle, BLUE);
    DrawCircleV(game.ballPos, BALL_RADIUS, WHITE);
    for (int i = 0; i < game.brickCount; i++) {
        if (game.bricks[i].width > 0) {
            DrawRectangleRec(game.bricks[i], RED);
            DrawRectangleLinesEx(game.bricks[i], 1, WHITE);
        }
    }

    DrawText(TextFormat("Score: %d", game.score), 20, 10, 20, YELLOW);
    DrawText(TextFormat("Lives: %d", game.lives), SCREEN_WIDTH - 120, 10, 20, GREEN);

    if (game.gameOver) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 200});
        DrawText("Game Over!", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 50, 50, RED);
        DrawText(TextFormat("Final Score: %d", game.score), SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 10, 30, YELLOW);
        DrawText("Press R to Restart", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 60, 20, WHITE);
    }
    if (game.gameWin) {
        DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (Color){0, 0, 0, 200});
        DrawText("You Win!", SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 - 50, 50, GREEN);
        DrawText(TextFormat("Final Score: %d", game.score), SCREEN_WIDTH/2 - 120, SCREEN_HEIGHT/2 + 10, 30, YELLOW);
        DrawText("Press R to Restart", SCREEN_WIDTH/2 - 100, SCREEN_HEIGHT/2 + 60, 20, WHITE);
    }

    EndDrawing();
}

// -------------------------- 主函数 --------------------------
int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "2D打砖块游戏");
    SetTargetFPS(FPS);
    InitGame();

    while (!WindowShouldClose()) {
        HandleInput();
        UpdateGame();
        DrawGame();
    }

    CloseWindow();
    return 0;
}
