#include "Game.h"
#include <raylib.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>

// 引入json命名空间
using json = nlohmann::json;

// 游戏初始化（修改后，从JSON读取所有参数）
void Game::Init(int width, int height, const char* title) {
    // 读取配置文件
    std::ifstream configFile("assets/config.json");
    if (!configFile.is_open()) {
        std::cerr << "配置文件读取失败！" << std::endl;
        return;
    }
    json config;
    configFile >> config;
    configFile.close();

    // 从配置读取游戏窗口参数
    windowWidth = config["game"]["windowWidth"];
    windowHeight = config["game"]["windowHeight"];
    windowTitle = config["game"]["title"];
    int maxLives = config["game"]["maxLives"];
    int targetFPS = config["game"]["targetFPS"];

    // 初始化窗口
    InitWindow(windowWidth, windowHeight, windowTitle.c_str());
    SetTargetFPS(targetFPS);

    // 初始化游戏基础数据
    score = 0;
    lives = maxLives;
    gameTime = 0.0f;
    currentState = GameState::MENU;

    // 从配置读取球拍参数，初始化
    float paddleWidth = config["paddle"]["width"];
    float paddleHeight = config["paddle"]["height"];
    float paddleSpeed = config["paddle"]["speed"];
    paddle.Init(
        {windowWidth/2.0f - paddleWidth/2.0f, windowHeight - 50.0f},
        paddleWidth, paddleHeight, paddleSpeed, windowWidth
    );

    // 从配置读取球参数，初始化
    float ballRadius = config["ball"]["radius"];
    Vector2 ballSpeed = {config["ball"]["initialSpeedX"], config["ball"]["initialSpeedY"]};
    ball.Init(
        {windowWidth/2.0f, windowHeight - 100.0f},
        ballSpeed, ballRadius, windowWidth, windowHeight
    );

    // 从配置读取砖块参数，初始化
    InitBricks(config);
}

// 生成砖块（修改后，从JSON读取参数）
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

