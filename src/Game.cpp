#include "Game.h"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

using json = nlohmann::json;

void Game::Init(int width, int height, const char* title) {
    std::ifstream configFile("assets/config.json");
    if (!configFile.is_open()) {
        std::cerr << "配置文件读取失败！" << std::endl;
        return;
    }
    json config;
    configFile >> config;
    configFile.close();

    windowWidth = config["game"]["windowWidth"];
    windowHeight = config["game"]["windowHeight"];
    windowTitle = config["game"]["title"];
    int maxLives = config["game"]["maxLives"];
    int targetFPS = config["game"]["targetFPS"];

    InitWindow(windowWidth, windowHeight, windowTitle.c_str());
    SetTargetFPS(targetFPS);

    score = 0;
    lives = maxLives;
    gameTime = 0.0f;
    currentState = GameState::MENU;

    float paddleWidth = config["paddle"]["width"];
    float paddleHeight = config["paddle"]["height"];
    float paddleSpeed = config["paddle"]["speed"];
    paddle.Init(
        {windowWidth/2.0f - paddleWidth/2.0f, windowHeight - 50.0f},
        paddleWidth, paddleHeight, paddleSpeed, windowWidth
    );

    float ballRadius = config["ball"]["radius"];
    Vector2 ballSpeed = {config["ball"]["initialSpeedX"], config["ball"]["initialSpeedY"]};
    ball.Init(
        {windowWidth/2.0f, windowHeight - 100.0f},
        ballSpeed, ballRadius, windowWidth, windowHeight
    );

    InitBricks(config);
}

void Game::InitBricks(const json& config) {
    bricks.clear();
    int rows = config["bricks"]["rows"];
    int cols = config["bricks"]["cols"];
    float brickWidth = config["bricks"]["width"];
    float brickHeight = config["bricks"]["height"];
    float padding = config["bricks"]["padding"];
    float offsetY = config["bricks"]["offsetY"];

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = j * (brickWidth + padding) + padding;
            float y = i * (brickHeight + padding) + offsetY;
            bricks.emplace_back(Rectangle{x, y, brickWidth, brickHeight});
        }
    }
}

void Game::HandleInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

void Game::Update() {
    gameTime += GetFrameTime();
    HandleInput();

    switch (currentState) {
        case GameState::MENU: UpdateMenu(); break;
        case GameState::PLAYING: UpdatePlaying(); break;
        case GameState::PAUSED: UpdatePaused(); break;
        case GameState::GAMEOVER: UpdateGameOver(); break;
    }
}

void Game::UpdateMenu() {
    if (IsKeyPressed(KEY_SPACE)) {
        currentState = GameState::PLAYING;
    }
}

void Game::UpdatePlaying() {
    if (IsKeyPressed(KEY_P)) {
        currentState = GameState::PAUSED;
        return;
    }

    paddle.Update(GetFrameTime());
    ball.Update(GetFrameTime());

    if (ball.CheckPaddleCollision(paddle)) {
        ball.ReverseSpeedY();
    }

    int activeBricks = 0;
    for (auto& brick : bricks) {
        if (brick.IsActive()) {
            activeBricks++;
            if (ball.CheckBrickCollision(brick)) {
                brick.Destroy();
                ball.ReverseSpeedY();
                score += 10;
                activeBricks--;
            }
        }
    }

    if (ball.GetPosition().y > windowHeight) {
        lives--;
        if (lives <= 0) {
            currentState = GameState::GAMEOVER;
        } else {
            ball.Reset(
                {windowWidth/2.0f, windowHeight - 100.0f},
                {300.0f, -300.0f}
            );
        }
    }
}

void Game::UpdatePaused() {
    if (IsKeyPressed(KEY_P)) {
        currentState = GameState::PLAYING;
    }
}

void Game::UpdateGameOver() {
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
}

void Game::ResetGame() {
    std::ifstream configFile("assets/config.json");
    json config;
    configFile >> config;
    configFile.close();

    score = 0;
    lives = config["game"]["maxLives"];
    gameTime = 0.0f;
    currentState = GameState::MENU;

    float paddleWidth = config["paddle"]["width"];
    float paddleHeight = config["paddle"]["height"];
    float paddleSpeed = config["paddle"]["speed"];
    paddle.Init(
        {windowWidth/2.0f - paddleWidth/2.0f, windowHeight - 50.0f},
        paddleWidth, paddleHeight, paddleSpeed, windowWidth
    );

    float ballRadius = config["ball"]["radius"];
    Vector2 ballSpeed = {config["ball"]["initialSpeedX"], config["ball"]["initialSpeedY"]};
    ball.Init(
        {windowWidth/2.0f, windowHeight - 100.0f},
        ballSpeed, ballRadius, windowWidth, windowHeight
    );

    InitBricks(config);
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    switch (currentState) {
        case GameState::MENU: DrawMenu(); break;
        case GameState::PLAYING: DrawPlaying(); break;
        case GameState::PAUSED: DrawPaused(); break;
        case GameState::GAMEOVER: DrawGameOver(); break;
    }

    EndDrawing();
}

void Game::DrawMenu() {
    DrawText("打砖块游戏", windowWidth/2 - 120, windowHeight/2 - 60, 40, WHITE);
    DrawText("按空格开始游戏", windowWidth/2 - 100, windowHeight/2 + 20, 20, GRAY);
    DrawText("按P暂停 | 按R重开 | ESC退出", windowWidth/2 - 150, windowHeight - 50, 20, DARKGRAY);
}

void Game::DrawPlaying() {
    paddle.Draw();
    ball.Draw();
    for (auto& brick : bricks) {
        brick.Draw();
    }

    DrawText(TextFormat("分数: %d", score), 20, 20, 20, WHITE);
    DrawText(TextFormat("生命值: %d", lives), windowWidth - 150, 20, 20, WHITE);
}

void Game::DrawPaused() {
    DrawPlaying();
    DrawRectangle(0, 0, windowWidth, windowHeight, {0, 0, 0, 180});
    DrawText("游戏暂停", windowWidth/2 - 100, windowHeight/2 - 30, 40, WHITE);
    DrawText("按P继续游戏", windowWidth/2 - 90, windowHeight/2 + 30, 20, GRAY);
}

void Game::DrawGameOver() {
    DrawText("游戏结束", windowWidth/2 - 120, windowHeight/2 - 60, 40, RED);
    DrawText(TextFormat("最终分数: %d", score), windowWidth/2 - 80, windowHeight/2, 25, WHITE);
    DrawText("按R重开游戏 | ESC退出", windowWidth/2 - 130, windowHeight/2 + 50, 20, GRAY);
}

bool Game::ShouldClose() {
    return WindowShouldClose();
}

void Game::Shutdown() {
    CloseWindow();
}

