//DIY推箱子小游戏
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
// #include <curses.h>
// #include <conio.h>

//宏定义
#define WIDTH   8
#define HEIGHT  8

void init();

void initDate();

void drawMap();

void moveUp();

void moveDown();

void moveLeft();

void moveRight();

//定义地图数组,二维数组有两个维度,地图是二维的矩形;
/*  0  表示空
    1  表示墙
    2  表示人
    3  表示箱子
    4  表示目的地
    5  表示已经完成的箱子*/

int map[WIDTH][HEIGHT] = {
        {0, 0, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 4, 1, 0, 0, 0},
        {0, 0, 1, 0, 1, 1, 1, 1},
        {1, 1, 1, 3, 0, 3, 4, 1},
        {1, 4, 0, 3, 2, 1, 1, 1},
        {1, 1, 1, 1, 3, 1, 0, 0},
        {0, 0, 0, 1, 4, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 0, 0}
};

//初始化人的位置;
int x = 0;
int y = 0;

//箱子的个数(没有推进去的)
int Box = 0;

//main函数
int main(void) {
    init();
    return 0;
}

void init() {
    //存储键盘按的方向
//    char direction = 0;
//     int direction = 0;

    //开始游戏,循环,每按一次循环一次;
    while (1) {
        //初始化一些数据---
        Box = 0;
        initDate();

        //每次循环的开始,清除一次屏幕;
        system("clear");

        //绘制地图;
        drawMap();

        //判断(未完成的)箱子的数量,为0时,game over!
        if (!Box) {
            break;
        }

//        //键盘输入方向,getch回显在控制台上
//         initscr();
////        垃圾xcode，弄了#include <curses.h>还是链接不起，烦死了
//         direction = getch();
//         endwin();
//
////        direction = getchar();

        fflush(stdin);//清空输入流缓冲区的字符;

        initscr();//初始化curses窗口,进入NCURSES模式.
        raw();//这个函数会阻止字符缓冲，令程序即时处理键盘输入，一些终端命令（中断[Ctrl-c],挂起[Ctrl-z]）也会被交给程序处理
        cbreak();//禁止行缓冲区
        noecho();//设置没有回显
        keypad(stdscr, TRUE);//识别用户的功能键，即F1、F2等这些键，代码中可以获得用户的这些按键
        curs_set(0);//调用会屏蔽掉物理指针
        int direction;//方向

        printf("请输入人物移动的方向...\n");
        direction = getch();
        if (direction == 'w' || direction == 'W' || direction == KEY_UP) {
            printw("up\n");
        } else if (direction == 's' || direction == 'S' || direction == KEY_DOWN) {
            printw("down\n");
        } else if (direction == 'a' || direction == 'A' || direction == KEY_LEFT) {
            printw("left\n");
        } else if (direction == 'd' || direction == 'D' || direction == KEY_RIGHT) {
            printw("right\n");
        } else if (direction == 'q' || direction == 'Q' || direction == '\x1b') {
            printw("nothing\n");
            break;
        } else {
            printw("nothing\n");
        }
        refresh();
        endwin();//关闭窗口,退出NCURSES模式。
//        scanf("%c%*c", &direction);
        fflush(stdin);//清空输入流缓冲区的字符;windows有效,linux无效?

//        printf("direction = %c\n", direction);
//        printf("src 0x%x dst 0x%x\n", direction, 'w');

        while ((direction != 'w') && (direction != 'a')
               && (direction != 's') && (direction != 'd')) {
            // putchar(direction);
            printf("无效输入,请再次输入...\n");
            // direction = getchar();
            fflush(stdin);//清空输入流缓冲区的字符;windows有效,linux无效?
            scanf("%c%*c", &direction);
//            printf("direction = %c\n", direction);
        }

        //判断用户的输入;
        switch (direction) {
            case 'w':
                //按下w时,上移
                moveLeft();
                break;
            case 's':
                //按下d时,下移
                // moveDown();
                moveRight();
                break;
            case 'a':
                //按下a时, 左移;
                // moveLeft();
                moveUp();
                break;
            case 'd':
                //按下d时, 右移;
                // moveRight();
                moveDown();
                break;
            default:
                printf("您的输入不合法: 请再次输入: \n");
                break;
        }
        system("clear");
    }//while
    printf("恭喜你, game over!\n");
}

//初始化数据;
void initDate() {
    printf("游戏加载中....\n");

    //遍历地图中的数据.
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            //遍历到人的时候.
            if (map[i][j] == 2) {
                //注意二维数组和数学中矩阵的对应.
                x = i;  //j -- 高度
                y = j;  //i -- 宽度
            }

            //未完成的箱子的数目
            if (map[i][j] == 3) {
                Box++;
            }
        }
    }

    printf("未推入箱子的个数为 %d , 当前人的坐标为( %d , %d )\n", Box, x, y);
}

//绘制地图;
void drawMap() {
    printf("开始绘制地图...\n");
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            switch (map[i][j]) {
                case 0:
                    printf("  ");
                    break;
                case 1:
                    printf("■ ");
                    break;
                case 2:
                    printf("♀ ");
                    break;
                case 3:
                    printf("∆ ");
                    break;
                case 4:
                    printf("▲ ");
                    break;
                case 5:
                    printf("★ ");
                    break;
            }
        }
        printf("\n");
    }
}

//向上移动
/**
 * 向上移动有几种情况.
 * 1、人前面为空白.
 *    这种情况有两步
 *    (1)将人当前位置设为空白  (0)
 *    (2)将人前面的那个位置设置为人  (2)
 * 2、人前面为箱子
 *    这种情况分3种情况.
 *    1、箱子前面为空白
 *        将人当前位置设为空白    (0)
 *        将箱子的位置设为人      (2)
 *        将箱子前面的空白设为未完成的箱子  (3)
 *    2、箱子前面是墙
 *        推不动,不做任何处理.
 *    3、箱子前面是终点
 *        将人的位置设为空白    (0)
 *        将箱子的位置设为人    (2)
 *        将终点的位置设为已经完成的箱子  (5)
 *        //箱子数减少1          Box--
 * 3、人前面是墙
 *    不用处理.
 * //4、人前面是已经完成的箱子-----在2里面包括了
 *    不处理-------
 */

//每次操作后只考虑能不能移动;能, 移动;不能, 不操作;
//每次循环都去获取人和未完成箱子的数据.----判断游戏是否退出
void moveUp() {
    //定义变量存放人物上方的坐标.
    int ux = x;
    int uy = y - 1;

    //人前面是墙
    if (map[ux][uy] == 1) {
        return;
    }

    //人前面是空白
    if (map[ux][uy] == 0) {
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        y = uy;
        return;
    }

    //人前面是箱子
    if ((map[ux][uy] == 3) || (map[ux][uy] == 5)) {
        //箱子的上一个位置
        int uux = x;
        int uuy = y - 2;

        //箱子前面是墙
        if (map[uux][uuy] == 1) {
            return;
        }

        //箱子前面是空白或者箱子前面是终点
        //更新map
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        y = uy;

        //箱子前面是空白
        if (map[uux][uuy] == 0) {
            map[uux][uuy] = 3;
            return;
        }

        //箱子前面是终点
        if (map[uux][uuy] == 4) {
            map[uux][uuy] = 5;
            return;
        }

    }
}

//向下移动
void moveDown() {
    //定义变量存放人物下方的坐标.
    int ux = x;
    int uy = y + 1;

    //人下面是墙
    if (map[ux][uy] == 1) {
        return;
    }

    //人下面是空白
    if (map[ux][uy] == 0) {
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        y = uy;
        return;
    }

    //人下面是箱子
    if (map[ux][uy] == 3 || map[ux][uy] == 5) {
        //箱子的下一个位置
        int uux = x;
        int uuy = y + 2;

        //箱子下面是墙
        if (map[uux][uuy] == 1) {
            return;
        }

        //箱子下面是空白
        //箱子下面是终点
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        y = uy;

        //箱子下面是空白
        if (map[uux][uuy] == 0) {
            map[uux][uuy] = 3;
            return;
        }

        //箱子下面是终点
        if (map[uux][uuy] == 4) {
            map[uux][uuy] = 5;
            return;
        }

    }

}

//向左移动
void moveLeft() {
    //定义变量存放人物左方的坐标.
    int ux = x - 1;
    int uy = y;

    //人左面是墙
    if (map[ux][uy] == 1) {
        return;
    }

    //人左面是空白
    if (map[ux][uy] == 0) {
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        x = ux;
        return;
    }

    //人左面是箱子
    if (map[ux][uy] == 3 || map[ux][uy] == 5) {
        //箱子的左一个位置
        int uux = x - 2;
        int uuy = y;
        // printf("111\n");
        //箱子左面是墙
        if (map[uux][uuy] == 1) {
            return;
        }

        //箱子左面是空白
        //箱子左面是终点
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        x = ux;

        //箱子左面是空白
        if (map[uux][uuy] == 0) {
            map[uux][uuy] = 3;
            return;
        }

        //箱子左面是终点
        if (map[uux][uuy] == 4) {
            map[uux][uuy] = 5;
            return;
        }

    }

}

//向右移动
void moveRight() {
    //定义变量存放人物右方的坐标.
    int ux = x + 1;
    int uy = y;

    //人右面是墙
    if (map[ux][uy] == 1) {
        return;
    }

    //人下右面是空白
    if (map[ux][uy] == 0) {
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        x = ux;
        return;
    }

    //人右面是箱子
    if (map[ux][uy] == 3 || map[ux][uy] == 5) {
        //箱子的下一个位置
        int uux = x + 2;
        int uuy = y;

        //箱子右面是墙
        if (map[uux][uuy] == 1) {
            return;
        }

        //箱子右面是空白
        //箱子右面是终点
        map[x][y] = 0;
        map[ux][uy] = 2;
        //更新人的坐标
        x = ux;

        //箱子下右面是空白
        if (map[uux][uuy] == 0) {
            map[uux][uuy] = 3;
            return;
        }

        //箱子右面是终点
        if (map[uux][uuy] == 4) {
            map[uux][uuy] = 5;
            return;
        }

    }
}