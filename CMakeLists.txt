cmake_minimum_required(VERSION 3.10)
project(Breakout LANGUAGES CXX)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找依赖库
find_package(raylib REQUIRED)
find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(GTest REQUIRED)

# 头文件目录
include_directories(include)

# 游戏源码
set(GAME_SOURCES
    src/main.cpp
    src/Game.cpp
    src/Ball.cpp
    src/Paddle.cpp
    src/Brick.cpp
)

# 生成游戏可执行文件
add_executable(Breakout ${GAME_SOURCES})
target_link_libraries(Breakout raylib nlohmann_json::nlohmann_json)


# 单元测试配置
enable_testing()
set(TEST_SOURCES
    tests/CollisionTest.cpp
    src/Ball.cpp
    src/Brick.cpp
    src/Paddle.cpp
)
add_executable(CollisionTest ${TEST_SOURCES})
target_include_directories(CollisionTest PRIVATE include)
target_link_libraries(CollisionTest GTest::GTest GTest::Main raylib)
add_test(NAME BallCollisionTest COMMAND CollisionTest)


