cmake_minimum_required(VERSION 3.10)
project(Breakout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到依赖库
find_package(raylib REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)

# 游戏主程序
add_executable(Breakout
    src/main.cpp
    src/Game.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Brick.cpp
    src/PowerUp.cpp
)

# 链接库
target_link_libraries(Breakout PRIVATE raylib nlohmann_json::nlohmann_json)

# 测试程序（保留你原来的）
add_executable(CollisionTest
    tests/CollisionTest.cpp
    src/Ball.cpp
    src/Brick.cpp
    src/Paddle.cpp
)
target_link_libraries(CollisionTest PRIVATE raylib GTest::GTest GTest::Main)
