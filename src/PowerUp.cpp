#include "PowerUp.h"
#include "Game.h"
#include "raymath.h"

// 基类通用方法
PowerUp::PowerUp(Vector2 pos, PowerUpType type, float duration, float fallSpeed)
    : position(pos), type(type), active(false), duration(duration), timer(0), fallSpeed(fallSpeed) {}

void PowerUp::UpdateFall(float dt) {
    position.y += fallSpeed * dt;
}

void PowerUp::Draw() {
    // 道具本体
    Color color;
    switch(type) {
        case PowerUpType::PADDLE_EXTEND: color = GREEN; break;
        case PowerUpType::MULTI_BALL: color = YELLOW; break;
        case PowerUpType::SLOW_BALL: color = BLUE; break;
    }
    DrawRectangleV(position, {20, 20}, color);
    // 作业要求的：道具掉落光晕特效
    DrawRing({position.x+10, position.y+10}, 15, 20, 0, 360, 32, Fade(color, 0.3f));
}

Rectangle PowerUp::GetRect() const {
    return {position.x, position.y, 20, 20};
}

// 工厂类实现
std::unique_ptr<PowerUp> PowerUpFactory::Create(PowerUpType type, Vector2 pos, const nlohmann::json& config) {
    float fallSpeed = config["powerup"]["fall_speed"];
    switch(type) {
        case PowerUpType::PADDLE_EXTEND:
            return std::make_unique<PaddleExtendPowerUp>(pos, config["powerup"]["paddle_extend"]["duration"], fallSpeed);
        case PowerUpType::SLOW_BALL:
            return std::make_unique<SlowBallPowerUp>(pos, config["powerup"]["slow_ball"]["duration"], fallSpeed);
        case PowerUpType::MULTI_BALL:
            return std::make_unique<MultiBallPowerUp>(pos, fallSpeed);
        default: return nullptr;
    }
}

// 加长板逻辑
PaddleExtendPowerUp::PaddleExtendPowerUp(Vector2 pos, float duration, float fallSpeed)
    : PowerUp(pos, PowerUpType::PADDLE_EXTEND, duration, fallSpeed) {}

void PaddleExtendPowerUp::Apply(Game& game) {
    originalWidth = game.paddle.width;
    game.paddle.width += addWidth;
    active = true;
}

void PaddleExtendPowerUp::Reset(Game& game) {
    game.paddle.width = originalWidth;
}

// 减速球逻辑
SlowBallPowerUp::SlowBallPowerUp(Vector2 pos, float duration, float fallSpeed)
    : PowerUp(pos, PowerUpType::SLOW_BALL, duration, fallSpeed) {}

void SlowBallPowerUp::Apply(Game& game) {
    originalSpeed = game.balls[0].speed;
    for(auto& ball : game.balls) {
        ball.speed *= speedMult;
    }
    active = true;
}

void SlowBallPowerUp::Reset(Game& game) {
    for(auto& ball : game.balls) {
        ball.speed = originalSpeed;
    }
}

// 多球逻辑
MultiBallPowerUp::MultiBallPowerUp(Vector2 pos, float fallSpeed)
    : PowerUp(pos, PowerUpType::MULTI_BALL, 0, fallSpeed) {}

void MultiBallPowerUp::Apply(Game& game) {
    for(int i=0; i<spawnCount; i++) {
        Ball newBall = game.balls[0];
        newBall.velocity = Vector2Rotate(newBall.velocity, DEG2RAD * GetRandomValue(-30, 30));
        game.balls.push_back(newBall);
    }
    active = true;
}

void MultiBallPowerUp::Reset(Game& game) {}
