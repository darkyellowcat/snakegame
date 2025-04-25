c++写了一个丑陋的小游戏，作为自己第一个github上的作品，就当把代码存放在这里。
如果有想尝试写贪吃蛇的佬可以当个思路，我自认为把注释写清楚了。
等哪天有兴趣了用easyx完善一下，目前就让它先这样丑陋着吧。

# Snake Game

A simple, "beautifully ugly" Snake game written in C++ as my first GitHub project. It's a classic console-based game, rough around the edges, but fully functional. Consider this a code stash for now—I'll polish it up someday, maybe with EasyX for a graphical makeover. Until then, it proudly wears its retro console charm!

## Features
- Classic Snake gameplay: control the snake, eat food, grow, and avoid crashing.
- Console-based interface using Windows APIs (`conio.h` and `windows.h`).
- WASD controls for movement, press X to exit.
- Score tracking (though the display might be a bit shy sometimes).

## How to Play
1. Use W/A/S/D to move the snake up/left/down/right.
2. Eat the food (`*`) to grow and increase your score.
3. Avoid hitting the walls or the snake's own body.
4. Press X to quit the game.

## Requirements
- **Windows OS** (due to `conio.h` and `windows.h` dependencies).
- A C++ compiler (e.g., g++, MSVC, or MinGW).
- CLion or another IDE (optional, for easy building).

## Build and Run
1. Clone the repository:
   ```bash
   git clone https://github.com/darkyellowcat/snakegame.git
   cd snakegame
