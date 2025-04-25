#include <bits/stdc++.h>
#include <conio.h>
#include <windows.h>
using namespace std;
typedef long long ll;
#define H 30
#define W 30

void menu();

// 设置窗口大小
void setConsoleSize() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SMALL_RECT rect = { 0, 0, W + 1, H + 3 }; // 32x33
    SetConsoleWindowInfo(hConsole, TRUE, &rect);
    COORD bufferSize = { W + 2, H + 3 };
    SetConsoleScreenBufferSize(hConsole, bufferSize);
}

class Game {
private:
    int score;      //得分
    int food_x, food_y;     //食物坐标
    int snake_x[100], snake_y[100]; //蛇的坐标
    int snake_len;      //蛇的长度
    int dir; // 控制方向  0: up, 1: right, 2: down, 3: left
    bool game_over;
    //不建议用system("cls")，屏幕让室友以为我在吃闪光弹，而且很费眼睛，建议用屏幕缓冲区
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);      //获取标准输出句柄
    CHAR_INFO screen[(H + 3) * (W + 2)];                // 屏幕缓冲区
    COORD bufferSize = { W + 2, H + 3 };
    COORD bufferCoord = { 0, 0 };
    SMALL_RECT writeRegion = { 0, 0, W + 1, H + 2 };    //留一行给score


    // 生成食物
    void generate_food() {
        do {
            food_x = rand() % W;
            food_y = rand() % H;
        } while (is_on_snake(food_x, food_y)); // 不能生成在蛇身上
    }

    // 判定
    bool is_on_snake(int x, int y) {
        for (int i = 0; i < snake_len; i++) {
            if (snake_x[i] == x && snake_y[i] == y) {
                return true;
            }
        }
        return false;
    }

    //生成画面
    void draw(char mp[H][W]) {
        // 初始化地图
        memset(mp, ' ', sizeof(char) * H * W);

        // 放置蛇
        mp[snake_y[0]][snake_x[0]] = '1'; // 蛇头
        for (int i = 1; i < snake_len; i++) {
            mp[snake_y[i]][snake_x[i]] = '0'; // 蛇身
        }

        // 放置食物
        mp[food_y][food_x] = '*';

        // 绘制地图+边框到 screen 一维缓冲区
        for (int i = 0; i < H + 3; i++) {
            for (int j = 0; j < W + 2; j++) {
                int index = i * (W + 2) + j;
                if (i == 0 || i == H + 1 ||i==H+2 || j == 0 || j == W + 1) {
                    screen[index].Char.AsciiChar = '#';
                } else {
                    screen[index].Char.AsciiChar = mp[i - 1][j - 1];
                }
                screen[index].Attributes = FOREGROUND_BLUE | FOREGROUND_INTENSITY;
            }
        }

        // 把分数写到最后一行（第 H + 2 行）
        string score_str = "Score: " + to_string(score);
        for (int i = 0; i < score_str.size() && i < W + 2; ++i) {
            int index = (H + 2) * (W + 2) + i;
            screen[index].Char.AsciiChar = score_str[i];
            screen[index].Attributes = FOREGROUND_RED | FOREGROUND_INTENSITY;
        }

        COORD bufferSize = { W + 2, H + 3 };
        SMALL_RECT writeRegion = { 0, 0, W + 1, H + 3 };
        // 输出缓冲区
        WriteConsoleOutputA(hOut, screen, bufferSize, bufferCoord, &writeRegion);
    }

    //输入选项
    void input() {
        if (_kbhit()) {
            switch (_getch()) {
                case 'w': if (dir != 2) dir = 0; break; // 上
                case 'd': if (dir != 3) dir = 1; break; // 右
                case 's': if (dir != 0) dir = 2; break; // 下
                case 'a': if (dir != 1) dir = 3; break; // 左
                case 'x': game_over = true; break; // 退出
            }
        }
    }

    // 运行逻辑
    void logic() {
        int prev_x = snake_x[0], prev_y = snake_y[0];
        int prev2_x, prev2_y;
        // 头
        switch (dir) {
            case 0: snake_y[0]--; break; // 上
            case 1: snake_x[0]++; break; // 右
            case 2: snake_y[0]++; break; // 下
            case 3: snake_x[0]--; break; // 左
        }
        // 身子
        for (int i = 1; i < snake_len; i++) {
            prev2_x = snake_x[i];
            prev2_y = snake_y[i];
            snake_x[i] = prev_x;
            snake_y[i] = prev_y;
            prev_x = prev2_x;
            prev_y = prev2_y;
        }
        // 吃到食物
        if (snake_x[0] == food_x && snake_y[0] == food_y) {
            score += 10;
            snake_len++;
            snake_x[snake_len - 1] = prev_x;
            snake_y[snake_len - 1] = prev_y;
            generate_food();
        }
        // 撞墙
        if (snake_x[0] < 0 || snake_x[0] >= W || snake_y[0] < 0 || snake_y[0] >= H) {
            game_over = true;
        }
        // 撞到自己
        for (int i = 1; i < snake_len; i++) {
            if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
                game_over = true;
            }
        }
    }

public:
    Game() {
        score = 0;
        snake_x[0] = W / 2; //自定义起始的位置
        snake_y[0] = H / 2;
        snake_len = 1;
        dir = 1;            //起始的方向
        game_over = false;
        generate_food();
        setConsoleSize();
    }

    void run() {
        char mp[H][W];
        while (!game_over) {
            draw(mp);
            input();
            logic();
            Sleep(200); // 蛇移动的速度（太大会出现延迟）
        }
        system("cls");      //这个时候要刷新一下，要不然结果会出现在地图里
        cout << "Game Over! Final Score: " << score << endl;
        cout << "Press any key to return to menu..." << endl;
        _getch();
        menu();
    }
};

void menu() {
    cout << "1. Start Game" << endl;
    cout << "2. Exit" << endl;
    cout << "Enter your choice: ";
    int choice;
    cin >> choice;
    switch (choice) {
        case 1: {
            Game game;
            game.run();
            break;
        }
        case 2:
            exit(0);
            break;
        default:
            cout << "Invalid choice" << endl;
            menu();
            break;
    }
}

int main() {
    srand(time(0)); // 随机生成食物
    menu();
    return 0;
}