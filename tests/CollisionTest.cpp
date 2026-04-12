#include <gtest/gtest.h>
#include "Ball.h"
#include "Brick.h"

// 测试用例1：球与砖块发生碰撞，返回true
TEST(BallCollisionTest, BallHitsBrick_ReturnsTrue) {
    Brick brick(Rectangle{100, 100, 50, 20});
    Ball ball;
    ball.Init({125, 110}, {300, -300}, 10, 800, 600);
    EXPECT_TRUE(ball.CheckBrickCollision(brick));
}

// 测试用例2：球与砖块无碰撞，返回false
TEST(BallCollisionTest, BallFarFromBrick_ReturnsFalse) {
    Brick brick(Rectangle{100, 100, 50, 20});
    Ball ball;
    ball.Init({500, 500}, {300, -300}, 10, 800, 600);
    EXPECT_FALSE(ball.CheckBrickCollision(brick));
}
