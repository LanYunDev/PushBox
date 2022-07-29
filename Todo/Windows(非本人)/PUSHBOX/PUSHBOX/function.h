#pragma once
#pragma warning (disable:4996)
#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<stdlib.h>
#include<graphics.h>//����easyx��
#include<Windows.h>
#include<mmsystem.h>//������ý���豸�ӿ�ͷ�ļ�
#include<string.h>
#include<conio.h>

#pragma comment(lib, "winmm.lib") //���ؾ�̬��


//������
void BGM();

void soundend();

void complete();

void jump();

void back();

void change();//�ı�ͼ�λ����洰�ڱ���

typedef struct {
    char UName[20];//�û�����������19���ַ���
    char UPassword[20];//�û������룬������19���ַ���
} UserInfo;

//��¼ϵͳ
int UserRegister();

int UserLogin();

//��ͼ�ṹ
typedef struct Map {
    char maps[50][50];
    int level;
    int m;//�������ʼ������
    int n;//�������ʼ������
    int aim_m[25];
    int aim_n[25];
    int aim_sum;

} Maps;


typedef struct//�˵�ϵͳ
{
    char judgeintf;
    char game_interf[50][50];
} Interfs;

//��������
typedef struct _undo {
    char r_map[50][50];
} undo;

extern Maps map;//��main�д�����ȫ�ֱ���������
extern Maps *gameMap(int level);//�ú�������Ϊ����txt�ļ��еĵ�ͼ���̶������в�������ָ��
extern Interfs Interf;//�����˵�
extern Interfs *interf(int scene);//�˵�ϵͳָ��
extern undo withdraw[1000];//��¼��ȥÿһ����ʱ�ĵ�ͼ
extern IMAGE img_all[7];//�������ز�


void record(Maps map, int step);//ʵʱ��¼����
void Undo(int step);//���ݲ�������ʵ���˲�����
void loadImg();//����ͼƬ
void init();//���ع��
char event();//���м�������ʱ ������ֵ���з��� �ﵽ�����������������
void update();//ʵʱ���µ�ͼ
void render();//�˵�������л����ܵ�ʵ��

FILE *openfile(char *, const char *);//�ļ����书�ܵ��ж��Ƿ�����ļ�

void initmap();//�µ�ͼ��ʼ����������
