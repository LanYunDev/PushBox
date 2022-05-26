//禁用fscanf的警告.因为在文件无误的情况下不存在问题，所以不需要警告.
#pragma clang diagnostic push
#pragma ide diagnostic ignored "cert-err34-c"
//推箱子小游戏 Powered By LanYun
//推箱子专业网站： http://sokoban.ws/
#include <ncurses.h>//对于代码内容关于ncurses.h库那些函数不太明白可以看看我写的[这篇文章](https://lanyundev.vercel.app/posts/a5945d21.html)噢
#include <stdlib.h>//声明了数值与字符串转换函数, 伪随机数生成函数, 动态内存分配函数, 进程控制函数等公共函数.
#include <locale.h> // 定义了特定地域的设置，实际上为了让程序在不同地域下运行，需要在程序开始处调用setlocale()函数。
#include <unistd.h> //sleep()函数
#include <errno.h> //errno函数,定义了透过错误码来回报错误信息的宏
#include <time.h> //time()函数,记录📝时间差

//宏定义
#define WIDTH   8 //游戏界面宽度
#define HEIGHT  8 //游戏界面高度
#define WORLD_WIDTH 50 //世界宽度
#define WORLD_HEIGHT 20 //世界高度

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

long Level = 1;//当前关卡
long Top_Level = 1;//最高关卡
long Latest_Level = 1;//最新关卡
long Total_Level = 14;//总关卡数
long Remain_Box = 1;//剩余箱子数
long Tmp = 0;//用于存储临时Level
int check;//退出标识的作用,及其检测是否有撤回操作
int x, y;//玩家位置
int ch;//获取输入内容
time_t Level_start, Level_end;//记录关卡开始和结束时间

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
    if (fp == NULL || check == 4) {//如果打开失败
        box(PushBox, 0, 0);//创建box窗口
        wrefresh(PushBox);//使box窗口生效
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 7, "未能打开或data.txt数据文件存在错误!");
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 4, offset_x + 8, "尝试自动生成默认data.txt数据文件!");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        fclose(fp);//关闭文件📃
        FILE *fp2 = fopen("data.txt", "w");//覆盖创建文件📃
        fputs("Top_Level:1\n"
              "Latest_Level:1", fp2);//导出数据
        fclose(fp2);//关闭文件📃
        sleep(1);//等待1秒
        goto FILE_FIX;//重新回到只读打开
    }
    if (fscanf(fp, "Top_Level:%ld Latest_Level:%ld", &Top_Level, &Latest_Level) !=
        2) {    //读取文件.⚠️：此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
        check = 4;//标志为文件📃需重新生成
        goto FILE_FIX;
    }
    fclose(fp);//关闭文件📃

    check = 0;//check标志置零

    //检查最高数据是否异常
    if (Top_Level < 1 || Top_Level > Total_Level) {
        box(PushBox, 0, 0);//创建box窗口
        wrefresh(PushBox);//使box窗口生效
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 5, offset_x + 9, "检测到最高数据异常,已自动归1 ");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        Top_Level = 1;//最高数据复位
        sleep(1);//等待1秒
    }
    if (Latest_Level < 1 || Latest_Level > Total_Level) {
        box(PushBox, 0, 0);//创建box窗口
        wrefresh(PushBox);//使box窗口生效
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 3, offset_x + 9, "检测到最新数据异常,已自动归1 ");
        refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        Latest_Level = 1;//最高数据复位
        sleep(1);//等待1秒
    }

//    游戏开始界面
    int i = 1, n = 0; //为下面的文字的移动声明变量
    while ((ch = getch()) != 'b') {
        wclear(PushBox);//清空PushBox窗口
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 8, offset_x + 18, "推箱子小游戏~");//绘制界面
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 1, offset_x + i + 6, "按B键开始游戏~");//绘制界面
        mvprintw(1, offset_x + WORLD_HEIGHT / 2, "上次记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);//绘制界面
        mvprintw(WORLD_HEIGHT - 3, offset_x + 10, "Copyright © ALL right reserved");//绘制界面
        mvprintw(WORLD_HEIGHT - 1, offset_x + 16, "Powered By LanYun");//绘制界面
        box(PushBox, 0, 0);//创建box窗口
        wrefresh(PushBox);//使box窗口生效
        //文字移动
        if (offset_x + i < WORLD_WIDTH - 12 && n % 2 == 0) {
            i++;
        } else if (i > 1 && n % 2 != 0) {
            i--;
        } else if (offset_x + i >= WORLD_WIDTH / 2 || i <= 1) {
            n++;
        } else {
        }
        // 如果读取到q就退出
        if (ch == 'q') {
            check = -1;
            return;
        }
    }
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    mvprintw(1, offset_x + WORLD_HEIGHT / 2, "上次记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);//绘制界面
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 13, "请输入开始🎮的关卡: ");//绘制界面

    // 获取输入的数字
    int Digits = 0; //Total_Level的位数
    int success; // 转换成功的标志
    //读取Total_Level位数
    for (long tmp = Total_Level; tmp; Digits++) {
        tmp /= 10;//tmp每次去掉最低一位
    }
    char input[Digits]; // input字符串数组

    do {
        mvprintw(offset_y + WORLD_HEIGHT / 2 - 1, offset_x + 10, "输入游玩🎮关卡数后按回车即可 ");//绘制界面
        mvprintw(offset_y + WORLD_HEIGHT / 2 + 4, offset_x + 8, "注意:输入需合法且不可高于最高记录 ");//绘制界面
        mvprintw(offset_y + WORLD_HEIGHT / 2 + 6, offset_x + 9, "若输入数据大于最高记录📝或小于1 ");//绘制界面
        mvprintw(offset_y + WORLD_HEIGHT / 2 + 8, offset_x + 10, "将自动采用最高纪录📝进行游玩 ");//绘制界面
        refresh();//将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
        if (!fgets(input, Digits + 1, stdin)) //最多读取Digits+1个字符数，其中包括换行符，如果没有读取任何东西，返回值要么是指向str指针，要么是NULL。
        {
            box(PushBox, 0, 0);//创建box窗口
            wrefresh(PushBox);//使box窗口生效
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 13, "请输入开始🎮的关卡: ");//绘制界面
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 4, offset_x + 13, "输入数据读取失败!");//绘制界面
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 2, offset_x + 16, "请在1秒后重试!");//绘制界面
            refresh();//刷新窗口
            sleep(1);//等待1秒
            continue;
        }
        // 将输入转换为整数:
        char *endptr;//定义endptr字符指针
        errno = 0; // 重置错误
        Level = strtol(input, &endptr, 10);//获取输入.并把数据附值给关卡.
        /*endptr设置为指向第一个无法转换的字符。因此，您可以检查整个字符串是否已转换。
         *base允许您指定您期望的数字中的任何base。大多数时候，这将是10，但在这里你可以给16来解析十六进制，或者给2来解析二进制。
         *strtol()甚至设置errno，因此您可以检查数字是太小还是太大，无法转换。*/
        if (errno == ERANGE || endptr == input) //ERANGE表示一个范围错误，它在输入参数超出数学函数定义的范围时发生，errno被设置为ERANGE || 没有读取任何字符
        {
            box(PushBox, 0, 0);//创建box窗口
            wrefresh(PushBox);//使box窗口生效
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 13, "请输入开始🎮的关卡: ");//绘制界面
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 4, offset_x + 12, "输入有误,请在1秒后重新输入 ");//绘制界面
            refresh();//将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
            sleep(1);//等待1秒
            success = 0;//未获得有效数字
        } else if (*endptr && (*endptr != '\n' && *endptr != '\r'))//*endptr既不是字符串的结尾也不是换行,所以我们没有转换*整个输入的内容
        {
            success = 0;//未获得有效数字
        } else {
            success = 1;//获得有效数字
        }
    } while (!success); // 重复,得到一个有效的数字.

    if (Level > Top_Level || Level < 1) {
        Level = Top_Level;
    }
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 12, "您即将开始🎮的关卡: %d", Level);
    refresh();//刷新窗口
    sleep(1);//等待1秒
}

void game() {
    Level_start = time(NULL);  //获取当前时间,单位为s

    Position *head = malloc(sizeof(Position));//创建一个头结点并分配内存
    Position *p = head;//声明一个指针指向头结点，用于遍历链表
    p->x = -1;//头结点的x坐标为0
    p->y = -1;//头结点的y坐标为0
    p->box_x = -1;//头结点的箱子x坐标为-1
    p->box_y = -1;//头结点的箱子y坐标为-1
    p->next = NULL;//设置下一个元素为空

    while (Level != (Total_Level + 1) && check != -1) {
        p = Draw(p);//绘制地图🗺️
        p = Move(head, p);//人物移动函数
        if (!Remain_Box) {
            Level++;//关卡数加一
            p = NULL;//指针指向空指针
            free(head);//释放指针空间
            head = malloc(sizeof(Position));//头结点分配内存
            p = head;//指针重新指向头结点，用于遍历链表
            win();//关卡通过✅函数
            Level_start = time(NULL);  //获取当前时间,单位为s
        }
    }
    free(head);//释放指针空间
}

Position *Draw(Position *p) {
    //判断check是否为3，如果是就不再执行
    if (check == 3)
        return p;

    //判断是否需要更新最高记录📝
    if (Level > Top_Level)
        Top_Level = Level;

    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量
    int offset_y = (LINES - WORLD_HEIGHT) / 2;//计算窗口的y轴偏移量
    Remain_Box = 0;

    box(PushBox, 0, 0);
    wrefresh(PushBox);
    mvprintw(1, offset_x + WORLD_HEIGHT / 2, "上次记录📝: %d，最高记录📝: %d", Latest_Level, Top_Level);//绘制界面
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 9, offset_x + 15, "请输入小人移动方向:");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 8, offset_x + 7, "(W:上,S:下,A:左,D:右,或者使用方向键)");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 7, offset_x + 4, "(按Q可保存数据并退出,按Z可回到上一步,仅一次)");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 + 5, offset_x + 7, "描述: ■ 代表墙 ♀ 代表小人 ∆ 代表箱子 ");//输入提示
    mvprintw(offset_y + WORLD_HEIGHT / 2 + 7, offset_x + 8, "▲ 代表目的地 ★ 代表箱子已到达目的地 ");//输入提示

    //在未进入下一关之前,文件📃仅读取一次.
    while (Tmp != Level) {
        //读入地图，并将地图中的数字转换为字符，存入地图数组中，方便绘制。
        FILE *fp = fopen("map.txt", "r");//打开文件
        if (fp == NULL) {
            FILE_Error:
            box(PushBox, 0, 0);//创建box窗口
            wrefresh(PushBox);//使box窗口生效
            mvprintw(offset_y + WORLD_HEIGHT / 2 - 6, offset_x + 7, "未能打开或map.txt地图文件存在错误!");
            refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
            sleep(1);//等待1秒
            exit(1);
        }
        fseek(fp, (16L * 8 + 1) * (Level - 1), SEEK_SET);//移动文件指针
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (fscanf(fp, "%1d", &map[i][j]) != 1) {    //读取文件.⚠️：此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
                    goto FILE_Error;
                }
            }
        }
        fclose(fp);//关闭文件
        Tmp = Level;
    }

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
            if (map[i][j] == 2 || map[i][j] == 6) {
                //注意二维数组和数学中矩阵的对应.
                if (check != 1)
                    p = add_position(p, j, i, -1, -1);
                x = j;//x坐标
                y = i;//y坐标
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
    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量

    //如果剩下的箱子数目为0，则说明本关卡通过✅,不再读取键盘输入并退出.
    if (!Remain_Box) {
        return p;
    }

    int direction;//方向键
    Input:
    Level_end = time(NULL);  //获取当前时间,单位为s
    mvprintw(WORLD_HEIGHT + 2, offset_x + 8, "当前关卡为: %ld , 本关卡已游玩🎮: %.0lf 秒 ", Level,
             difftime(Level_end, Level_start));//绘制界面,输出关卡号和游玩关卡时间
    direction = getch();//读取键盘输入
    //判断方向
    if (direction == 'w' || direction == 'W' || direction == KEY_UP) {//上
        moveUp(p);//向上移动
        check = 0;
        return p;//返回当前位置
    } else if (direction == 's' || direction == 'S' || direction == KEY_DOWN) {//下
        moveDown(p);//向下移动
        check = 0;
        return p;//返回当前位置
    } else if (direction == 'a' || direction == 'A' || direction == KEY_LEFT) {//左
        moveLeft(p);//向左移动
        check = 0;
        return p;//返回当前位置
    } else if (direction == 'd' || direction == 'D' || direction == KEY_RIGHT) {//右
        moveRight(p);//向右移动
        check = 0;
        return p;//返回当前位置
    } else if (direction == 'q' || direction == 'Q' || direction == '\x1b') {//退出
        check = -1;//将check赋值为-1，然后退出
        return p;//返回当前位置
    } else if (direction == 'z' || direction == 'Z') {
        if (check != 3) {
            return moveBack(head, p);
        } else goto Input;
    } else {
        goto Input;//如果输入未知，则跳回去重新读取输入.
    }
}

void game_over() {
    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量
    int offset_y = (LINES - WORLD_HEIGHT) / 2;//计算窗口的y轴偏移量

    //关卡记录更新
    Latest_Level = Level;

    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 7, offset_x + WORLD_WIDTH / 2 - 8, "推箱子小游戏结束 ");//绘制游戏🎮结束界面
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 5, offset_x + WORLD_WIDTH / 2 - 15, "本次记录📝: %d  最高记录📝: %d", Latest_Level,
             Top_Level);//绘制界面,输出本次游玩🎮的关卡数和最高关卡数
    FILE *fp = fopen("data.txt", "w");//打开文件
    if (fp == NULL) {//判断文件是否存在
        printf("打开data.txt文件失败！");
        exit(1);
    }
    fprintf(fp, "Top_Level:%ld\nLatest_Level:%ld", Top_Level, Latest_Level);//此处，处在一处安全🔐性警告⚠️，但因无能修复而不得不使用。
    fclose(fp);//关闭文件
    mvprintw(offset_y + WORLD_HEIGHT / 2 - 3, offset_x + WORLD_WIDTH / 2 - 15, "数据保存成功！ 3秒后自动退出～ ");//绘制界面
    mvprintw(WORLD_HEIGHT - 3, offset_x + 10, "Copyright © ALL right reserved");//绘制界面
    mvprintw(WORLD_HEIGHT - 1, offset_x + 16, "Powered By LanYun");//绘制界面
    refresh();  //将虚拟屏幕上的内容写到显示屏上，并且刷新窗口
    sleep(3);//等待3秒
    delwin(PushBox);//删除box窗口
    endwin();//结束窗口
}

void win() {
    box(PushBox, 0, 0);//创建box窗口
    wrefresh(PushBox);//使box窗口生效
    int offset_x = (COLS - WORLD_WIDTH) / 2;//计算窗口的x轴偏移量
    int offset_y = (LINES - WORLD_HEIGHT) / 2;//计算窗口的y轴偏移量
    mvprintw(offset_y + WORLD_HEIGHT / 2, offset_x + WORLD_WIDTH / 2 - 15, "恭喜🎉通关！快进入下一关吧～👉 ");//绘制界面
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
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
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
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
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
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
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
        if (map[y][x] == 6) {
            map[y][x] = 4;
        } else map[y][x] = 0;
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

        //箱子右面是墙,完成的箱子，箱子📦就返回
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

    int n = 0;//记录📝指针循环♻️次数

    while (tmp->next->next != NULL) {//移动指针到倒数第二个
        tmp = tmp->next;//链表tmp指向下一个节点
        n++;//计数n增加一
    }

    //判断步数是否有两步以上，如果有就不进行撤回
    if (!n) {
        free(del);//释放指针del的内存

        check = 3;//不可再进行撤回标识

        return p;
    }

    del = tmp->next; //要删除的位置,即当前位置
    tmp->next = NULL;//删除tmp的下一个节点
    p = tmp; //要撤回到的位置

    //如果有相同的坐标，直接删除一个。
    if ((p->y == del->y) && (p->x == del->x)) {
        free(del);//释放指针del的内存

        check = 1;//可再进行撤回标识

        return p;
    }

    if ((p->box_x > 0) && (p->box_y > 0)) {   //如果人物移动前有箱子📦的话
        //撤回人物的移动。
        if (map[p->y][p->x] == 4) {//如果被撤回的位置是目的地
            map[p->y][p->x] = 6;//是，则修改人人在目的地
        } else map[p->y][p->x] = 2;//否，则修改为人的位置

        //下面👇的功能为判断移动方向，并清除移动过的箱子📦
        int x_ = del->x - p->x;//规定向右👉为正，用于计算箱子📦被移动后的位置
        int y_ = del->y - p->y;//规定向下👇为正，用于计算箱子📦被移动后的位置

        //将箱子📦移动后的位置恢复为上次移动的状态
        if (map[p->box_y + y_][p->box_x + x_] == 5)//判断将被撤回箱子📦的位置是否为为目的地
            map[p->box_y + y_][p->box_x + x_] = 4;//是，则将被撤回的位置修改为目的地
        else map[p->box_y + y_][p->box_x + x_] = 0;//如果不是，则修改为空

        //撤回箱子📦的移动
        if (map[p->box_y][p->box_x] == 4 || map[p->box_y][p->box_x] == 6)//判断箱子📦撤回到的位置是否为含有目的地
            map[p->box_y][p->box_x] = 5;//是，则将被撤回的位置修改为已完成✅的箱子📦
        else map[p->box_y][p->box_x] = 3;//如果不是，则修改为箱子📦

    } else {  //如果人物移动前没有箱子📦

        //撤回人物的移动。
        if (map[p->y][p->x] == 4) {//如果被撤回的位置是目的地
            map[p->y][p->x] = 6;//是，则修改人人在目的地
        } else map[p->y][p->x] = 2;//否，则修改为人的位置

        //将人物移动后的位置恢复为上次移动的状态
        if (map[del->y][del->x] == 6) {//如果被删除位置是目的地
            map[del->y][del->x] = 4;//是，则将被删除的位置修改为目的地
        } else map[del->y][del->x] = 0;//如果不是，则修改为空白
    }

    free(del);//释放指针del的内存

    check = 1;

    return p;
}

/*  0  表示空
    1  表示墙
    2  表示人
    3  表示箱子
    4  表示目的地
    5  表示已经完成的箱子
    6  表示人在目的地
    */

//Todo
















#pragma clang diagnostic pop


/*代码保护*/
// 程序出Bug了？
// 　　　∩∩
// 　　（´･ω･）
// 　 ＿|　⊃／(＿＿_
// 　／ └-(＿＿＿／
// 　￣￣￣￣￣￣￣
// 算了反正不是我写的
// 　　 ⊂⌒／ヽ-、＿
// 　／⊂_/＿＿＿＿ ／
// 　￣￣￣￣￣￣￣
// 万一是我写的呢
// 　　　∩∩
// 　　（´･ω･）
// 　 ＿|　⊃／(＿＿_
// 　／ └-(＿＿＿／
// 　￣￣￣￣￣￣￣
// 算了反正改了一个又出三个
// 　　 ⊂⌒／ヽ-、＿
// 　／⊂_/＿＿＿＿ ／
// 　￣￣￣￣￣￣￣

//                            _ooOoo_
//                           o8888888o
//                           88" . "88
//                           (| -_- |)
//                            O\ = /O
//                        ____/`---'\____
//                      .   ' \\| |// `.
//                       / \\||| : |||// \
//                     / _||||| -:- |||||- \
//                       | | \\\ - /// | |
//                     | \_| ''\---/'' | |
//                      \ .-\__ `-` ___/-. /
//                   ___`. .' /--.--\ `. . __
//                ."" '< `.___\_<|>_/___.' >'"".
//               | | : `- \`.;`\ _ /`;.`/ - ` : | |
//                 \ \ `-. \_ __\ /__ _/ .-` / /
//         ======`-.____`-.___\_____/___.-`____.-'======
//                            `=---='
//
//         .............................................
//               佛祖保佑       永不宕机     永无BUG
//          佛曰:
//                  写字楼里写字间，写字间里程序员；
//                  程序人员写程序，又拿程序换酒钱。
//                  酒醒只在网上坐，酒醉还来网下眠；
//                  酒醉酒醒日复日，网上网下年复年。
//                  但愿老死电脑间，不愿鞠躬老板前；
//                  奔驰宝马贵者趣，公交自行程序员。
//                  别人笑我忒疯癫，我笑自己命太贱；
//                  不见满街漂亮妹，哪个归得程序员？

/*
 *                                                     __----~~~~~~~~~~~------___
 *                                    .  .   ~~//====......          __--~ ~~
 *                    -.            \_|//     |||\\  ~~~~~~::::... /~
 *                 ___-==_       _-~o~  \/    |||  \\            _/~~-
 *         __---~~~.==~||\=_    -_--~/_-~|-   |\\   \\        _/~
 *     _-~~     .=~    |  \\-_    '-~7  /-   /  ||    \      /
 *   .~       .~       |   \\ -_    /  /-   /   ||      \   /
 *  /  ____  /         |     \\ ~-_/  /|- _/   .||       \ /
 *  |~~    ~~|--~~~~--_ \     ~==-/   | \~--===~~        .\
 *           '         ~-|      /|    |-~\~~       __--~~
 *                       |-~~-_/ |    |   ~\_   _-~            /\
 *                            /  \     \__   \/~                \__
 *                        _--~ _/ | .-~~____--~-/                  ~~==.
 *                       ((->/~   '.|||' -_|    ~~-/ ,              . _||
 *                                  -_     ~\      ~~---l__i__i__i--~~_/
 *                                  _-~-__   ~)  \--______________--~~
 *                                //.-~~~-~_--~- |-------~~~~~~~~
 *                                       //.-~~~--\
 *                       ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 *
 *                               神兽保佑            永无BUG
 */

/*
 * ......................................&&.........................
 * ....................................&&&..........................
 * .................................&&&&............................
 * ...............................&&&&..............................
 * .............................&&&&&&..............................
 * ...........................&&&&&&....&&&..&&&&&&&&&&&&&&&........
 * ..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............
 * ................&...&&&&&&&&&&&&&&&&&&&&&&&&&&&&.................
 * .......................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........
 * ...................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...............
 * ..................&&&   &&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
 * ...............&&&&&@  &&&&&&&&&&..&&&&&&&&&&&&&&&&&&&...........
 * ..............&&&&&&&&&&&&&&&.&&....&&&&&&&&&&&&&..&&&&&.........
 * ..........&&&&&&&&&&&&&&&&&&...&.....&&&&&&&&&&&&&...&&&&........
 * ........&&&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&....&&&.......
 * .......&&&&&&&&.....................&&&&&&&&&&&&&&&&.....&&......
 * ........&&&&&.....................&&&&&&&&&&&&&&&&&&.............
 * ..........&...................&&&&&&&&&&&&&&&&&&&&&&&............
 * ................&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&............
 * ..................&&&&&&&&&&&&&&&&&&&&&&&&&&&&..&&&&&............
 * ..............&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&....&&&&&............
 * ...........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&......&&&&............
 * .........&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&.........&&&&............
 * .......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&............
 * ......&&&&&&&&&&&&&&&&&&&...&&&&&&...............&&&.............
 * .....&&&&&&&&&&&&&&&&............................&&..............
 * ....&&&&&&&&&&&&&&&.................&&...........................
 * ...&&&&&&&&&&&&&&&.....................&&&&......................
 * ...&&&&&&&&&&.&&&........................&&&&&...................
 * ..&&&&&&&&&&&..&&..........................&&&&&&&...............
 * ..&&&&&&&&&&&&...&............&&&.....&&&&...&&&&&&&.............
 * ..&&&&&&&&&&&&&.................&&&.....&&&&&&&&&&&&&&...........
 * ..&&&&&&&&&&&&&&&&..............&&&&&&&&&&&&&&&&&&&&&&&&.........
 * ..&&.&&&&&&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&&&&&&&&&&&&.......
 * ...&&..&&&&&&&&&&&&.........&&&&&&&&&&&&&&&&...&&&&&&&&&&&&......
 * ....&..&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&...........&&&&&&&&.....
 * .......&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&..............&&&&&&&....
 * .......&&&&&.&&&&&&&&&&&&&&&&&&..&&&&&&&&...&..........&&&&&&....
 * ........&&&.....&&&&&&&&&&&&&.....&&&&&&&&&&...........&..&&&&...
 * .......&&&........&&&.&&&&&&&&&.....&&&&&.................&&&&...
 * .......&&&...............&&&&&&&.......&&&&&&&&............&&&...
 * ........&&...................&&&&&&.........................&&&..
 * .........&.....................&&&&........................&&....
 * ...............................&&&.......................&&......
 * ................................&&......................&&.......
 * .................................&&..............................
 * ..................................&..............................
 */


