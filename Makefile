CC = gcc
CFLAGS = -Wall -std=c99
# 补全所有Raylib需要的链接库，彻底解决链接报错
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# 生成的游戏程序名
TARGET = brick_breaker
# 源代码文件
SRC = brick_breaker.c

all: $(TARGET)

# 编译命令 ⚠️ 这一行开头必须用Tab键缩进，不能用空格！
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET) $(LIBS)

# 运行游戏 ⚠️ 这一行开头必须用Tab键缩进，不能用空格！
run: $(TARGET)
	./$(TARGET)

# 清理编译产物 ⚠️ 这一行开头必须用Tab键缩进，不能用空格！
clean:
	rm -f $(TARGET) *.o



