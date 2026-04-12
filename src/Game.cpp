#include "Game.h"
#include <raylib.h>
#include <iostream>

// 游戏初始化
void Game::Init(int width, int height, const char* title) {
    windowWidth = width;
    windowHeight = height;
    windowTitle = title;

    // 初始化窗口
    InitWindow(windowWidth, windowHeight, windowTitle.c_str());
    SetTargetFPS(60);

    // 初始化游戏基础数据
    score = 0;
    lives = 3;
    gameTime = 0.0f;
    currentState = GameState::MENU; // 初始状态为菜单

    // 初始化球拍
    paddle.Init(
        {windowWidth/2.0f - 60.0f, windowHeight - 50.0f},
        120.0f, 20.0f, 400.0f, windowWidth
    );

    // 初始化球
    ball.Init(
        {windowWidth/2.0f, windowHeight - 100.0f},
        {300.0f, -300.0f}, 10.0f, windowWidth, windowHeight
    );

    // 初始化砖块
    InitBricks();
}

// 生成砖块
void Game::InitBricks() {
    bricks.clear();
    int rows = 5;
    int cols = 10;
    float brickWidth = 70.0f;
    float brickHeight = 20.0f;
    float padding = 5.0f;
    float offsetY = 50.0f;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            float x = j * (brickWidth + padding) + padding;
            float y = i * (brickHeight + padding) + offsetY;
            bricks.emplace_back(Rectangle{x, y, brickWidth, brickHeight});
        }
    }
}

// 全局输入处理
void Game::HandleInput() {
    // 全局退出
    if (IsKeyPressed(KEY_ESCAPE)) {
        CloseWindow();
    }
}

// 状态更新总入口（状态机核心，用switch替代bool嵌套）
void Game::Update() {
    gameTime += GetFrameTime();
    HandleInput();

    // 状态机分支，完全替代原有的if (!gameOver && !paused) 嵌套
    switch (currentState) {
        case GameState::MENU: UpdateMenu(); break;
        case GameState::PLAYING: UpdatePlaying(); break;
        case GameState::PAUSED: UpdatePaused(); break;
        case GameState::GAMEOVER: UpdateGameOver(); break;
    }
}

// 菜单状态更新
void Game::UpdateMenu() {
    // 按空格进入游戏
    if (IsKeyPressed(KEY_SPACE)) {
        currentState = GameState::PLAYING;
    }
}

// 游戏中状态更新（核心游戏逻辑）
void Game::UpdatePlaying() {
    // 按P暂停游戏
    if (IsKeyPressed(KEY_P)) {
        currentState = GameState::PAUSED;
        return;
    }

    // 更新球拍和球
    paddle.Update(GetFrameTime());
    ball.Update(GetFrameTime());

    // 球拍碰撞检测
    if (ball.CheckPaddleCollision(paddle)) {
        ball.ReverseSpeedY();
    }

    // 砖块碰撞检测
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

    // 球掉出屏幕，扣生命值
    if (ball.GetPosition().y > windowHeight) {
        lives--;
        if (lives <= 0) {
            currentState = GameState::GAMEOVER;
        } else {
            // 重置球的位置
            ball.Reset(
                {windowWidth/2.0f, windowHeight - 100.0f},
                {300.0f, -300.0f}
            );
        }
    }
}

// 暂停状态更新
void Game::UpdatePaused() {
    // 按P继续游戏
    if (IsKeyPressed(KEY_P)) {
        currentState = GameState::PLAYING;
    }
}

// 游戏结束状态更新
void Game::UpdateGameOver() {
    // 按R重置游戏
    if (IsKeyPressed(KEY_R)) {
        ResetGame();
    }
}

// 游戏重置
void Game::ResetGame() {
    score = 0;
    lives = 3;
    gameTime = 0.0f;
    currentState = GameState::MENU;

    // 重置球拍、球、砖块
    paddle.Init(
        {windowWidth/2.0f - 60.0f, windowHeight - 50.0f},
        120.0f, 20.0f, 400.0f, windowWidth
    );
    ball.Init(
        {windowWidth/2.0f, windowHeight - 100.0f},
        {300.0f, -300.0f}, 10.0f, windowWidth, windowHeight
    );
    InitBricks();
}

// 绘制总入口
void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    // 状态机绘制分支
    switch (currentState) {
        case GameState::MENU: DrawMenu(); break;
        case GameState::PLAYING: DrawPlaying(); break;
        case GameState::PAUSED: DrawPaused(); break;
        case GameState::GAMEOVER: DrawGameOver(); break;
    }

    EndDrawing();
}

// 菜单绘制
void Game::DrawMenu() {
    DrawText("打砖块游戏", windowWidth/2 - 120, windowHeight/2 - 60, 40, WHITE);
    DrawText("按空格开始游戏", windowWidth/2 - 100, windowHeight/2 + 20, 20, GRAY);
    DrawText("按P暂停 | 按R重开 | ESC退出", windowWidth/2 - 150, windowHeight - 50, 20, DARKGRAY);
}

// 游戏中绘制
void Game::DrawPlaying() {
    // 绘制游戏元素
    paddle.Draw();
    ball.Draw();
    for (auto& brick : bricks) {
        brick.Draw();
    }

    // 绘制UI
    DrawText(TextFormat("分数: %d", score), 20, 20, 20, WHITE);
    DrawText(TextFormat("生命值: %d", lives), windowWidth - 150, 20, 20, WHITE);
}

// 暂停绘制
void Game::DrawPaused() {
    // 保留游戏画面作为背景
    DrawPlaying();
    // 半透明遮罩
    DrawRectangle(0, 0, windowWidth, windowHeight, {0, 0, 0, 180});
    DrawText("游戏暂停", windowWidth/2 - 100, windowHeight/2 - 30, 40, WHITE);
    DrawText("按P继续游戏", windowWidth/2 - 90, windowHeight/2 + 30, 20, GRAY);
}

// 游戏结束绘制
void Game::DrawGameOver() {
    DrawText("游戏结束", windowWidth/2 - 120, windowHeight/2 - 60, 40, RED);
    DrawText(TextFormat("最终分数: %d", score), windowWidth/2 - 80, windowHeight/2, 25, WHITE);
    DrawText("按R重开游戏 | ESC退出", windowWidth/2 - 130, windowHeight/2 + 50, 20, GRAY);
}

// 窗口关闭判断
bool Game::ShouldClose() {
    return WindowShouldClose();
}

// 游戏资源释放
void Game::Shutdown() {
    CloseWindow();
}
