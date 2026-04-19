#pragma once
#include "raylib.h"
#include "nlohmann/json.hpp"
#include <memory>

// 前置声明
class Game;

// 道具类型枚举（作业要求的3种）
enum class PowerUpType {
    PADDLE_EXTEND, // 加长板
    MULTI_BALL,    // 多球
    SLOW_BALL      // 减速球
};

// 道具抽象基类（工厂模式核心）
class PowerUp {
public:
    Vector2 position;
    PowerUpType type;
    bool active;
    float duration;
    float timer;
    float fallSpeed;

    PowerUp(Vector2 pos, PowerUpType type, float duration, float fallSpeed);
    virtual ~PowerUp() = default;

    void UpdateFall(float dt);
    void Draw();
    Rectangle GetRect() const;

    // 子类实现各自的效果
    virtual void Apply(Game& game) = 0;
    virtual void Reset(Game& game) = 0;
};

// 道具工厂类（作业要求的工厂模式）
class PowerUpFactory {
public:
    static std::unique_ptr<PowerUp> Create(PowerUpType type, Vector2 pos, const nlohmann::json& config);
};

// 加长板道具
class PaddleExtendPowerUp : public PowerUp {
public:
    PaddleExtendPowerUp(Vector2 pos, float duration, float fallSpeed);
    void Apply(Game& game) override;
    void Reset(Game& game) override;
private:
    float addWidth = 40.0f;
    float originalWidth = 0;
};

// 减速球道具
class SlowBallPowerUp : public PowerUp {
public:
    SlowBallPowerUp(Vector2 pos, float duration, float fallSpeed);
    void Apply(Game& game) override;
    void Reset(Game& game) override;
private:
    float speedMult = 0.6f;
    float originalSpeed = 0;
};

// 多球道具
class MultiBallPowerUp : public PowerUp {
public:
    MultiBallPowerUp(Vector2 pos, float fallSpeed);
    void Apply(Game& game) override;
    void Reset(Game& game) override;
private:
    int spawnCount = 2;
};