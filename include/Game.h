#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <raylib.h>
#include <vector>
#include <string>

// 游戏状态枚举（完整实现4种要求的状态）
enum class GameState {
    MENU,       // 菜单
    PLAYING,    // 游戏中
    PAUSED,     // 暂停
    GAMEOVER    // 游戏结束
};

class Game {
private:
    // 游戏核心成员（原全局变量全部封装）
    Ball ball;
    Paddle paddle;
    std::vector<Brick> bricks;
    int score;
    int lives;
    float gameTime;
    GameState currentState;

    // 窗口配置
    int windowWidth;
    int windowHeight;
    std::string windowTitle;

    // 内部私有方法
    void InitBricks();
    void HandleInput();
    void UpdateMenu();
    void UpdatePlaying();
    void UpdatePaused();
    void UpdateGameOver();
    void DrawMenu();
    void DrawPlaying();
    void DrawPaused();
    void DrawGameOver();

public:
    // 游戏生命周期核心方法
    void Init(int width, int height, const char* title);
    void Update();
    void Draw();
    void Shutdown();
    bool ShouldClose();
    void ResetGame();
};

#endif // GAME_H
