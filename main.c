//推箱子小游戏 Powered By LanYun
#include <ncurses.h>//对于代码内容关于ncurses.h库那些函数不太明白可以看看我写的[这篇文章](https://lanyundev.vercel.app/posts/a5945d21.html)噢
#include <stdlib.h>
#include <locale.h> // 定义了特定地域的设置，实际上为了让程序在不同地域下运行，需要在程序开始处调用setlocale()函数。
#include <unistd.h> //sleep()函数

//宏定义
#define WIDTH   8
#define HEIGHT  8
#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

//建立链表，用于存储箱子的位置，方便玩家回到上一个位置。
typedef struct Position {//定义一个结构体，用于存储箱子的位置
    int x, y;//x,y坐标
    int box_x, box_y;//箱子的坐标
    struct Position *next;//指向下一个结点
} Position;

//链表添加元素函数
Position *add_position(Position *p, int x, int y, int box_x, int box_y) {//p为链表头指针，x,y为玩家位置，box_x,box_y为箱子位置
    Position *tmp = (Position *) malloc(sizeof(Position));//分配内存
    tmp->x = x;//设置元素值
    tmp->y = y;//设置元素值
    tmp->box_x = box_x;//设置元素值
    tmp->box_y = box_y;//设置元素值
    tmp->next = NULL;//设置下一个元素为空
    p->next = tmp;//将新元素添加到链表中
    p = p->next;//将p指向新元素
    return p;//返回新元素
}

void Init();//初始化函数

void game();//游戏函数

Position *Draw(Position *p);//绘制函数

Position *Move(Position *head, Position *p);//移动函数

void moveUp(Position *p);//向上移动函数

void moveDown(Position *p);//向下移动函数

void moveLeft(Position *p);//向左移动函数

void moveRight(Position *p);//向右移动函数

Position *moveBack(Position *head, Position *p);//回退函数

void win();//完成一关✅函数

void game_over();//游戏结束函数

WINDOW *PushBox;//定义推箱子窗口

int Level = 1;//当前关卡
int Top_Level = 1;//最高关卡
int Latest_Level = 1;//最新关卡
int Remain_Box = 1;//剩余箱子数
int ch;//获取输入内容
int Tmp = 0;//用于存储临时Level
int check = 0;//退出标识的作用
int x, y;//玩家位置
int Total_Level = 14;//总关卡数

//定义地图数组,二维数组有两个维度,地图是二维的矩形;
/*  0  表示空
    1  表示墙
    2  表示人
    3  表示箱子
    4  表示目的地
    5  表示已经完成的箱子
    6  表示人在目的地
    */

int map[HEIGHT][WIDTH] = {0};//地图数组

int main(void) {
    Init();//初始化
    game();//游戏
    game_over();//游戏结束
    return EXIT_SUCCESS;//成功退出程序
}

void Init() {
    //初始化窗口
    setlocale(LC_ALL, "");// 为了curses界面显示中文
    initscr();//初始化curses窗口,进入NCURSES模式.
    raw();//这个函数会阻止字符缓冲，令程序即时处理键盘输入，一些终端命令（中断[Ctrl-c],挂起[Ctrl-z]）也会被交给程序处理
    cbreak();//禁止行缓冲区
    timeout(400);//设置超时时间
    noecho();//设置没有回显
    keypad(stdscr, TRUE);//识别用户的功能键，即F1、F2等这些键，代码中可以获得用户的这些按键
    curs_set(0);//调用会屏蔽掉物理指针
    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量
    int offset_y = (LINES - WORLD_HEIGHT) / 2;//计算窗口的y轴偏移量
    PushBox = newwin(WORLD_HEIGHT, WORLD_WIDTH, offset_y, offset_x);//创建窗口--newwin(高,宽,离上顶的高度,离左边的距离)
    FILE_FIX:
    sleep(0);//等待0秒
    FILE *fp = fopen("data.txt", "r");//只读打开data.txt文件📃
    if (fp == NULL) {//如果打开失败
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 8, "未能打开或不存在data.txt数据文件!");
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 4, offset_x + 8, "尝试自动生成默认data.txt数据文件!");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        fclose(fp);//关闭文件📃
        FILE *fp2 = fopen("data.txt", "w");//覆盖创建文件📃
        fputs("Top_Level:3\n"
              "Latest_Level:1", fp2);//导出数据
        fclose(fp2);//关闭文件📃
        sleep(1);//等待1秒
        goto FILE_FIX;//重新回到只读打开
    }
    fscanf(fp, "Top_Level:%d Latest_Level:%d", &Top_Level, &Latest_Level);//读取文件.⚠️：此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
    fclose(fp);//关闭文件📃

    //检查最高数据是否异常
    if (Top_Level < 1 || Top_Level > Total_Level) {
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 5, offset_x + 9, "检测到最高数据异常,已自动归1 ");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        Top_Level = 1;//最高数据复位
        sleep(1);//等待1秒
    }
    if (Latest_Level < 1 || Latest_Level > Total_Level) {
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 3, offset_x + 9, "检测到最新数据异常,已自动归1 ");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        Latest_Level = 1;//最高数据复位
        sleep(1);//等待1秒
    }

    int i = 1, n = 0;
    while ((ch = getch()) != 'b') {
        wclear(PushBox);
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 8, offset_x + 18, "推箱子小游戏~");
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 1, offset_x + i + 6, "按B键开始游戏~");
        mvprintw(1, offset_x + WORLD_HEIGHT / 2, "最新记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);
        mvprintw(WORLD_HEIGHT - 3, offset_x + 10, "Copyright © ALL right reserved");
        mvprintw(WORLD_HEIGHT - 1, offset_x + 16, "Powered By LanYun");
        box(PushBox, 0, 0);
        wrefresh(PushBox);
        if (offset_x + i < WORLD_WIDTH - 12 && n % 2 == 0) {
            i++;
        } else if (i > 1 && n % 2 != 0) {
            i--;
        } else if (offset_x + i >= WORLD_WIDTH / 2 || i <= 1) {
            n++;
        } else {
        }
        if (ch == 'q') {
            check = -1;
            return;
        }
    }
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    mvprintw(1, offset_x + WORLD_HEIGHT / 2, "最新记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 16, "请输入开始的关卡: %d", Latest_Level);
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 4, offset_x + 8, "(不可大于最高记录,默认最新记录开始)");
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 2, offset_x + 7, "(大于最高记录或小于0,将从最高记录开始)");
    refresh();//将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    Level = ((int) getchar() - 48);//读取关卡
    if (Level > Top_Level || Level < 1) {
        Level = Top_Level;
    }
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 16, "请输入开始的关卡: %d", Level);
    refresh();//刷新窗口
}

void game() {
    Position *head = malloc(sizeof(Position));//创建一个头结点并分配内存
    Position *p = head;//声明一个指针指向头结点，用于遍历链表
    p->x = 0;//头结点的x坐标为0
    p->y = 0;//头结点的y坐标为0
    p->box_x = -1;//头结点的箱子x坐标为-1
    p->box_y = -1;//头结点的箱子y坐标为-1
    p->next = NULL;//设置下一个元素为空

    while (Level != (Total_Level + 1) && check != -1) {
        p = Draw(p);
        p = Move(head, p);
        if (!Remain_Box) {
            Level++;
            free(head);//释放指针空间
            head = malloc(sizeof(Position));//头结点分配内存
            win();
        }
    }
    free(head);//释放指针空间
}

Position *Draw(Position *p) {
    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量
    int offset_y = (LINES - WORLD_HEIGHT) / 2;//计算窗口的y轴偏移量
    Remain_Box = 0;

    //关卡记录更新
    Latest_Level = Level;
    if (Level > Top_Level)
        Top_Level = Level;

    box(PushBox, 0, 0);
    wrefresh(PushBox);
    mvprintw(1, offset_x + WORLD_HEIGHT / 2, "最新记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 9, offset_x + 15, "请输入小人移动方向:");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 8, offset_x + 7, "(W:上,S:下,A:左,D:右,或者使用方向键)");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 7, offset_x + 4, "(按Q可保存数据并退出,按Z可回到上一步,仅一次)");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 + 5, offset_x + 7, "描述: ■ 代表墙 ♀ 代表小人 ∆ 代表箱子 ");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 + 7, offset_x + 8, "▲ 代表目的地 ★ 代表箱子已到达目的地 ");//输入提示

    while (Tmp != Level) {
        //读入地图，并将地图中的数字转换为字符，存入地图数组中，方便绘制。
        FILE *fp = fopen("map.txt", "r");//打开文件
        if (fp == NULL) {
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 10, "未能打开或不存在map.txt地图文件!");
            refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
            sleep(1);//等待1秒
            exit(1);
        }
        fseek(fp, (16L * 8 + 1) * (Level - 1), SEEK_SET);//移动文件指针
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                fscanf(fp, "%1d", &map[i][j]); //读取文件.⚠️：此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
            }
        }
        fclose(fp);//关闭文件
        Tmp = Level;
    }

    wclear(PushBox);
    //绘制地图🗺️
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            switch (map[i][j]) {
                case 0:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "  ");
                    break;
                case 1:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "■ ");
                    break;
                case 2:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "♀ ");
                    break;
                case 3:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "∆ ");
                    break;
                case 4:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "▲ ");
                    break;
                case 5:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "★ ");
                    break;
                case 6:
                    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5 + 1 * i, offset_x + 17 + 2 * j, "♀ ");
                    break;
            }
            //遍历到人的时候.
            if (map[i][j] == 2) {
                //注意二维数组和数学中矩阵的对应.
                p = add_position(p, j, i, -1, -1);
                x = j;
                y = i;
            }
            //未完成的箱子的数目
            if (map[i][j] == 3) {
                Remain_Box++;
            }
        }
    }
    refresh(); //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    return p;
}

Position *Move(Position *head, Position *p) {
    if (!Remain_Box) {
        return p;
    }

    int direction;//方向键
    Input:
    direction = getch();//读取键盘输入
    //判断方向
    if (direction == 'w' || direction == 'W' || direction == KEY_UP) {
        moveUp(p);
        return p;
    } else if (direction == 's' || direction == 'S' || direction == KEY_DOWN) {
        moveDown(p);
        return p;
    } else if (direction == 'a' || direction == 'A' || direction == KEY_LEFT) {
        moveLeft(p);
        return p;
    } else if (direction == 'd' || direction == 'D' || direction == KEY_RIGHT) {
        moveRight(p);
        return p;
    } else if (direction == 'q' || direction == 'Q' || direction == '\x1b') {
        check = -1;
        return p;
    } else if (direction == 'z' || direction == 'Z') {
        return moveBack(head, p);
    } else {
        goto Input;
    }
}

void game_over() {
    int offset_x = (COLS - WORLD_WIDTH) / 2;
    int offset_y = (LINES - WORLD_HEIGHT) / 2;

    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 7, offset_x + WORLD_WIDTH / 2 - 8, "推箱子小游戏结束 ");
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5, offset_x + WORLD_WIDTH / 2 - 15, "最新记录📝: %d  最高记录📝: %d", Latest_Level,
             Top_Level);
    FILE *fp = fopen("data.txt", "w");
    if (fp == NULL) {
        printf("打开data.txt文件失败！");
        exit(1);
    }
    fprintf(fp, "Top_Level:%d\nLatest_Level:%d", Top_Level, Latest_Level);//此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
    fclose(fp);
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 3, offset_x + WORLD_WIDTH / 2 - 15, "数据保存成功！ 3秒后自动退出～ ");
    mvprintw(WORLD_HEIGHT - 3, offset_x + 10, "Copyright © ALL right reserved");
    mvprintw(WORLD_HEIGHT - 1, offset_x + 16, "Powered By LanYun");
    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    sleep(3);//等待3秒
    delwin(PushBox);
    endwin();
}

void win() {

//    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    int offset_x = (COLS - WORLD_WIDTH) / 2;
    int offset_y = (LINES - WORLD_HEIGHT) / 2;
    mvprintw(offset_y + WORLD_HEIGHT / 2, offset_x + WORLD_WIDTH / 2 - 15, "恭喜🎉通关！快进入下一关吧～👉 ");
    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    sleep(1);//等待1秒
}

//向上移动
/**
 * 向上移动有几种情况.
 * 1、人上面为空白.
 *    这种情况有两步
 *    (1)将人当前位置设为空白  (0)
 *    (2)将人上面的那个位置设置为人  (2)
 * 2、人上面为箱子
 *    这种情况分3种情况.
 *    1、箱子上面为空白
 *        将人当前位置设为空白    (0)
 *        将箱子的位置设为人      (2)
 *        将箱子上面的空白设为未完成的箱子  (3)
 *    2、箱子上面是墙
 *        推不动,不做任何处理.
 *    3、箱子上面是终点
 *        将人的位置设为空白    (0)
 *        将箱子的位置设为人    (2)
 *        将终点的位置设为已经完成的箱子  (5)
 *        //箱子数减少1          Box--
 *     4、人上面是目的地
 * 3、人上面是墙
 *          不用处理.
 * 4、人上面是已经完成的箱子-----在2里面包括了
 *            不处理-------
 *
 */

//每次操作后只考虑能不能移动;能, 移动;不能, 不操作;
//每次循环都去获取人和未完成箱子的数据.----判断游戏是否退出

void moveUp(Position *p) {
    //定义变量存放人物上方的坐标.
    int ux = x;
    int uy = y - 1;

    //人上面是墙
    if (map[uy][ux] == 1) {
        return;
    }

    //人上面是空白
    if (map[uy][ux] == 0) {
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        map[uy][ux] = 2;
        //更新人的坐标
        y = uy;
        return;
    }

    //人上面是目的地
    if (map[uy][ux] == 4) {
        map[y][x] = 0;
        map[uy][ux] = 6;
        //更新人的坐标
        y = uy;
        return;
    }

    //人上面是箱子
    if ((map[uy][ux] == 3) || (map[uy][ux] == 5)) {
        //箱子的上一个位置
        int uux = x;
        int uuy = y - 2;

        //箱子上面是墙
        if (map[uuy][uux] == 1 || map[uuy][uux] == 5 || map[uuy][uux] == 3) {
            return;
        }

        //箱子位置记录📝
        p->box_x = ux;
        p->box_y = uy;

        //箱子上面是空白或者箱子上面是终点
        //更新map
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        if (map[uy][ux] == 5) {
            map[uy][ux] = 6;
        } else map[uy][ux] = 2;
        //更新人的坐标
        y = uy;

        //箱子上面是空白
        if (map[uuy][uux] == 0) {
            map[uuy][uux] = 3;
            return;
        }

        //箱子上面是终点
        if (map[uuy][uux] == 4) {
            map[uuy][uux] = 5;
            return;
        }

    }
}

//向下移动
void moveDown(Position *p) {
    //定义变量存放人物下方的坐标.
    int ux = x;
    int uy = y + 1;

    //人下面是墙
    if (map[uy][ux] == 1) {
        return;
    }

    //人下面是空白
    if (map[uy][ux] == 0) {
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        map[uy][ux] = 2;
        //更新人的坐标
        y = uy;
        return;
    }

    //人下面是目的地
    if (map[uy][ux] == 4) {
        map[y][x] = 0;
        map[uy][ux] = 6;
        //更新人的坐标
        y = uy;
        return;
    }

    //人下面是箱子
    if (map[uy][ux] == 3 || map[uy][ux] == 5) {
        //箱子的下一个位置
        int uux = x;
        int uuy = y + 2;

        //箱子下面是墙
        if (map[uuy][uux] == 1 || map[uuy][uux] == 5 || map[uuy][uux] == 3) {
            return;
        }

        //箱子位置记录📝
        p->box_x = ux;
        p->box_y = uy;

        //箱子下面是空白或终点
        //更新map
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        if (map[uy][ux] == 5) {
            map[uy][ux] = 6;
        } else map[uy][ux] = 2;
        //更新人的坐标
        y = uy;

        //箱子下面是空白
        if (map[uuy][uux] == 0) {
            map[uuy][uux] = 3;
            return;
        }

        //箱子下面是终点
        if (map[uuy][uux] == 4) {
            map[uuy][uux] = 5;
            return;
        }

    }

}

//向左移动
void moveLeft(Position *p) {
    //定义变量存放人物左方的坐标.
    int ux = x - 1;
    int uy = y;

    //人左面是墙
    if (map[uy][ux] == 1) {
        return;
    }

    //人左面是空白
    if (map[uy][ux] == 0) {
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        map[uy][ux] = 2;
        //更新人的坐标
        x = ux;
        return;
    }

    //人左面是目的地
    if (map[uy][ux] == 4) {
        map[y][x] = 0;
        map[uy][ux] = 6;
        //更新人的坐标
        x = ux;
        return;
    }

    //人左面是箱子
    if (map[uy][ux] == 3 || map[uy][ux] == 5) {
        //箱子的左一个位置
        int uux = x - 2;
        int uuy = y;

        //箱子左面是墙或箱子
        if (map[uuy][uux] == 1 || map[uuy][uux] == 5 || map[uuy][uux] == 3) {
            return;
        }

        //箱子位置记录📝
        p->box_x = ux;
        p->box_y = uy;

        //箱子左面是空白或终点
        //更新map
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        if (map[uy][ux] == 5) {
            map[uy][ux] = 6;
        } else map[uy][ux] = 2;
        //更新人的坐标
        x = ux;

        //箱子左面是空白
        if (map[uuy][uux] == 0) {
            map[uuy][uux] = 3;
            return;
        }

        //箱子左面是终点
        if (map[uuy][uux] == 4) {
            map[uuy][uux] = 5;
            return;
        }

    }

}

//向右移动
void moveRight(Position *p) {
    //定义变量存放人物右方的坐标.
    int ux = x + 1;
    int uy = y;

    //人右面是墙
    if (map[uy][ux] == 1) {
        return;
    }

    //人右面是空白
    if (map[uy][ux] == 0) {
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        map[uy][ux] = 2;
        //更新人的坐标
        x = ux;
        return;
    }

    //人右面是目的地
    if (map[uy][ux] == 4) {
        map[y][x] = 0;
        map[uy][ux] = 6;
        //更新人的坐标
        x = ux;
        return;
    }

    //人右面是箱子
    if (map[uy][ux] == 3 || map[uy][ux] == 5) {

        //箱子的下一个位置
        int uux = x + 2;
        int uuy = y;

        //箱子右面是墙
        if (map[uuy][uux] == 1 || map[uuy][uux] == 5 || map[uuy][uux] == 3) {
            return;
        }

        //箱子位置记录📝
        p->box_x = ux;
        p->box_y = uy;

        //箱子右面是空白或终点
        //更新map
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
        if (map[uy][ux] == 5) {
            map[uy][ux] = 6;
        } else map[uy][ux] = 2;
        //更新人的坐标
        x = ux;

        //箱子右面是空白
        if (map[uuy][uux] == 0) {
            map[uuy][uux] = 3;
            return;
        }

        //箱子右面是终点
        if (map[uuy][uux] == 4) {
            map[uuy][uux] = 5;
            return;
        }

    }
}

//撤回移动
Position *moveBack(Position *head, Position *p) {
    Position *tmp = head;
    Position *del = NULL;

    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    del = tmp->next;
    tmp->next = NULL;
    p = tmp;

    if ((p->box_x > 0) && (p->box_y > 0)) {
        //撤回人物和撤回箱子📦移动，（顺带覆盖掉了人物）
        if (map[del->y][del->x] == 6) {
            map[del->y][del->x] = 5;
        } else map[del->y][del->x] = 3;
        map[p->y][p->x] = 2;

        //清除移动过的箱子📦
        //判断移动方向
        int x_ = del->x - p->x;//规定向右👉为正
        int y_ = del->y - p->y;//规定向下👇为正

        if (map[p->box_y + y_][p->box_x + x_] == 5)
            map[p->box_y + y_][p->box_x + x_] = 4;
        else if (x_ != y_)
            map[p->box_y + y_][p->box_x + x_] = 0;

    } else {
        if (map[del->y][del->x] == 6) {
            map[del->y][del->x] = 4;
        } else map[del->y][del->x] = 0;
        map[p->y][p->x] = 2;
    }

    free(del);

    return p;
}





































