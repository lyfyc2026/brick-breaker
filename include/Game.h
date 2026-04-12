#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Paddle.h"
#include "Brick.h"
#include <raylib.h>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

// 游戏状态枚举
enum class GameState {
    MENU,
    PLAYING,
    PAUSED,
    GAMEOVER
};

class Game {
private:
    Ball ball;
    Paddle paddle;
    std::vector<Brick> bricks;
    int score;
    int lives;
    float gameTime;
    GameState currentState;

    int windowWidth;
    int windowHeight;
    std::string windowTitle;

    void InitBricks(const nlohmann::json& config);
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
    void Init(int width, int height, const char* title);
    void Update();
    void Draw();
    void Shutdown();
    bool ShouldClose();
    void ResetGame();
};

#endif // GAME_H
