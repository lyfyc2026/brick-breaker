#include "Game.h"

int main() {
    // 创建游戏实例
    Game game;
    // 初始化窗口与游戏
    game.Init(800, 600, "Breakout 打砖块");

    // 极简主循环
    while (!game.ShouldClose()) {
        game.Update();
        game.Draw();
    }

    // 释放资源
    game.Shutdown();
    return 0;
}
