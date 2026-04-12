# Breakout 打砖块游戏
C++ + Raylib 实现的经典打砖块游戏，面向对象重构，完整的游戏状态机，支持JSON配置文件。

## 项目功能
1.  完整的游戏流程：菜单界面、游戏中、暂停、游戏结束4种状态
2.  面向对象重构：Game、Ball、Paddle、Brick四大类封装，消除全局变量
3.  配置化：所有游戏参数通过`assets/config.json`配置，无需重新编译即可修改
4.  单元测试：基于GoogleTest实现碰撞检测的单元测试，覆盖核心逻辑

## 编译运行
### 环境依赖
- Ubuntu/Debian 系统
- C++17 及以上
- Raylib 图形库
- nlohmann-json 配置库
- GoogleTest 测试框架

安装依赖命令：
```bash
sudo apt update
sudo apt install libraylib-dev nlohmann-json3-dev libgtest-dev libgmock-dev cmake g++ -y
