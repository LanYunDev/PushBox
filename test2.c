#include <stdio.h>
#include <stdlib.h>
//#include <ncurses.h>
//#include <locale.h> // 定义了特定地域的设置，实际上为了让程序在不同地域下运行，需要在程序开始处调用setlocale()函数。
#include <time.h>

//宏定义
#define WIDTH   8
#define HEIGHT  8
#define WORLD_WIDTH 50
#define WORLD_HEIGHT 20

//WINDOW *PushBox;//定义推箱子窗口

int Level = 1;//当前关卡
int Top_Level = 1;//最高关卡
int Latest_Level = 1;//最新关卡
int Remain_Box = 1;//剩余箱子数
int ch;//获取输入内容
int Tmp = 0;//用于存储临时Level
int check = 1;//判断是否移动过
int x, y;

typedef struct Link {
    int elem;
    struct Link *next;
} link;

//link * initLink();
//链表插入的函数，p是链表，elem是插入的结点的数据域，add是插入的位置
link *insertElem(link *p, int elem, int add);

//删除结点的函数，p代表操作链表，add代表删除节点的位置
link *delElem(link *p, int add);

//查找结点的函数，elem为目标结点的数据域的值
int selectElem(link *p, int elem);

//更新结点的函数，newElem为新的数据域的值
link *amendElem(link *p, int add, int newElem);

void display(link *p);

//建立链表，用于存储箱子的位置，方便玩家回到上一个位置。
typedef struct Position {
    int x, y;
    int box_x, box_y;
    struct Position *next;
} Position;

Position *add_position(Position *p, int x, int y, int box_x, int box_y) {
    Position *tmp = (Position *) malloc(sizeof(Position));
    tmp->x = x;
    tmp->y = y;
    tmp->box_x = box_x;
    tmp->box_y = box_y;
    tmp->next = NULL;
    p->next = tmp;
    p = p->next;
    return p;
}
//地图数组

Position *draw(Position *p) {
    p = add_position(p, 1, 1, 1, 1);
    p = add_position(p, 1, 2, 1, 2);
    p = add_position(p, 1, 3, 1, 3);
//    head =add_position(head, 1, 4, 1, 4);
    return p;
}

Position *moveDown(Position *head, Position *p) {
    Position *tmp = head;
    Position *del = NULL;

    p->x++;
    p->box_x++;
    printf("%d %d %d %d\n", p->x, p->y, p->box_x, p->box_y);
    while (tmp->next->next != NULL) {
        tmp = tmp->next;
    }
    del = tmp->next;
    tmp->next = NULL;
    p = tmp;

    free(del);
    printf("%d %d %d %d\n", p->x, p->y, p->box_x, p->box_y);
    p = add_position(p, 5, 5, 5, 5);
    return p;
}

Position *move(Position *head, Position *p) {
//    head = head->next;
//    printf("%d %d %d %d\n",head->x,head->y,head->box_x,head->box_y);
    return moveDown(head, p);
}

link *initLink(int elem) {
    link *p = (link *) malloc(sizeof(link));
    p->elem = elem;
    p->next = NULL;
    return p;
}

int main() {
//    link * p = (link*)malloc(sizeof(link));//创建一个头结点
//    link * temp = p;//声明一个指针指向头结点，用于遍历链表
//    int i = 0;
//    //生成链表
//    for (i = 1; i < 5; i++) {
//        //创建节点并初始化
//
//        //建立新节点与直接前驱节点的逻辑关系
//        temp->next = initLink(i);
//        temp = temp->next;
//    }

    Position *head = malloc(sizeof(Position));//创建一个头结点并分配内存
    Position *p = head;//声明一个指针指向头结点，用于遍历链表
    p->x = 0;//头结点的x坐标为0
    p->y = 0;//头结点的y坐标为0
    p->box_x = -1;//头结点的箱子x坐标为-1
    p->box_y = -1;//头结点的箱子y坐标为-1
    p->next = NULL;
    p = draw(p);
    p = move(head, p);
    getchar();

//    link *p = NULL;
//    int address;
//    //初始化链表（1，2，3，4）
//    printf("初始化链表为：\n");
//    p = initLink();
//    display(p);
//
//    printf("在第4的位置插入元素5：\n");
//    p = insertElem(p, 5, 4);
//    display(p);
//
//    printf("删除元素3:\n");
//    p = delElem(p, 3);
//    display(p);
//
//    printf("查找元素2的位置为：\n");
//    address = selectElem(p, 2);
//    if (address == -1) {
//        printf("没有该元素");
//    }
//    else {
//        printf("元素2的位置为：%d\n", address);
//    }
//    printf("更改第3的位置上的数据为7:\n");
//    p = amendElem(p, 3, 7);
//    display(p);

    return 0;
}

//link * initLink() {
//    link * p = (link*)malloc(sizeof(link));//创建一个头结点
//    link * temp = p;//声明一个指针指向头结点，用于遍历链表
//    int i = 0;
//    //生成链表
//    for (i = 1; i < 5; i++) {
//        link *a = (link*)malloc(sizeof(link));
//        a->elem = i;
//        a->next = NULL;
//        temp->next = a;
//        temp = temp->next;
//    }
//    return p;
//}
link *insertElem(link *p, int elem, int add) {
    link *temp = p;//创建临时结点temp
    link *c = NULL;
    int i = 0;
    //首先找到要插入位置的上一个结点
    for (i = 1; i < add; i++) {
        if (temp == NULL) {
            printf("插入位置无效\n");
            return p;
        }
        temp = temp->next;
    }
    //创建插入结点c
    c = (link *) malloc(sizeof(link));
    c->elem = elem;
    //向链表中插入结点
    c->next = temp->next;
    temp->next = c;
    return p;
}

link *delElem(link *p, int add) {
    link *temp = p;
    link *del = NULL;
    int i = 0;
    //遍历到被删除结点的上一个结点
    for (i = 1; i < add; i++) {
        temp = temp->next;
    }
    del = temp->next;//单独设置一个指针指向被删除结点，以防丢失
    temp->next = temp->next->next;//删除某个结点的方法就是更改前一个结点的指针域
    free(del);//手动释放该结点，防止内存泄漏
    return p;
}

int selectElem(link *p, int elem) {
    link *t = p;
    int i = 1;
    while (t->next) {
        t = t->next;
        if (t->elem == elem) {
            return i;
        }
        i++;
    }
    return -1;
}

link *amendElem(link *p, int add, int newElem) {
    int i = 0;
    link *temp = p;
    temp = temp->next;//tamp指向首元结点
    //temp指向被删除结点
    for (i = 1; i < add; i++) {
        temp = temp->next;
    }
    temp->elem = newElem;
    return p;
}

void display(link *p) {
    link *temp = p;//将temp指针重新指向头结点
    //只要temp指针指向的结点的next不是Null，就执行输出语句。
    while (temp->next) {
        temp = temp->next;
        printf("%d ", temp->elem);
    }
    printf("\n");
}

////启动日志
//void Log(){time_t start;//定义时间变量
//    FILE *fp = fopen("log.txt", "w");//打开文件
//    if (fp == NULL) {
//        printf("打开文件失败！");
//        exit(1);
//    }
//    fprintf(fp, "推箱子小游戏 Powered By LanYun\n");
//    fprintf(fp, "游戏开始\n");
//    fprintf(fp, "游戏时间：%s\n", asctime(localtime(&start)));
//    fprintf(fp, "游戏地图：\n");
//    for (int i = 0; i < WIDTH; i++) {
//        for (int j = 0; j < HEIGHT; j++) {
//            fprintf(fp, "%d ", map[i][j]);
//        }
//        fprintf(fp, "\n");
//    }
//    fclose(fp);//关闭文件
//}