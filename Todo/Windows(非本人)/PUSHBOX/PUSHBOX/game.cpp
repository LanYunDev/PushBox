#include"function.h"

int step = 0;
int scene = 2;//Ϊ2�Ͳ���ӡ��ֹ�û�������������ʱ��Ҳ��ӡ��ͼ
int is_scene = 0;//��is_scene=1���ӡinterface,����render�ж�
int level = 1;//�ؿ���
int x, y;


Maps *temp = &map;//ָ���ͼ ͨ��temp�������еĳ�Ա

void BGM() {
    mciSendString("open Mario.mp3", 0, 0, 0);

    mciSendString("play Mario.mp3 repeat", 0, 0, 0);


}

void complete() {
    mciSendString("open love.mp3", 0, 0, 0);

    mciSendString("play love.mp3", 0, 0, 0);


}

void soundend() {

    mciSendString("open end.mp3", 0, 0, 0);

    mciSendString("play end.mp3", 0, 0, 0);


}

void jump() {

    mciSendString("open jump.mp3", 0, 0, 0);

    mciSendString("play jump.mp3", 0, 0, 0);


}

void back() {
    mciSendString("open back.mp3", 0, 0, 0);

    mciSendString("play back.mp3", 0, 0, 0);


}


void init() {
    //�öδ��빦�������ع�꣬������win32��̽ӿ�
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);//��ȡ���ھ��
    CONSOLE_CURSOR_INFO cci;//ʵ�л�һ������̨�����Ϣ��
    GetConsoleCursorInfo(hOut, &cci);//��ȡ�����Ϣ
    cci.bVisible = FALSE;//���ع��
    SetConsoleCursorInfo(hOut, &cci);//���ù����Ϣ
    SetConsoleTitle("�����ι������Ģ��");
    //system("������");//�趨��������
    system("mode con cols=120 lines=30");//�趨���ڴ�С
}

void change() {
    //��ȡ���ھ��
    HWND hnd = GetHWnd();
    //���ô��ڱ���
    SetWindowText(hnd, "����µ�Ģ����������������మ");

}

void initmap()//�ú���Ч��Ϊ��ʼ���������(�ؿ��� ������)
{

    //level = 1;
    temp = gameMap(level);
    temp->level = level;
    x = temp->n;//nΪ��
    y = temp->m;
    step = 0;
}

void loadImg()//�ú���Ч��Ϊ���������ӵ��ز�ͼƬ������������ȫ�ֱ���img_all[7]��
{

    for (int i = 0; i < 7; i++) {
        char file[20] = "";

        sprintf(file, "../images/%d.png", i);//��ȡ�زķ������� �ٴ��������������ȫ�ֱ���

        loadimage(img_all + i, file, 64, 64);

    }

}

Maps *gameMap(int level)//�ú�������Ϊ����txt�ļ��еĵ�ͼ���̶������в�������ָ��
{
    Maps *temp = &map;
    char gmap[256];
    FILE *fp;
    sprintf(gmap, "../Map/%d.txt", level);//��ȡ�ļ���ȡ�ؿ���Ϣ��������
    fp = openfile(gmap, "r");

    temp->aim_sum = 0;
    for (int i = 0;; i++) {
        fgets(temp->maps[i], 256, fp);//�ļ�ָ�� ���ļ������л�ȡ
        temp->maps[i][strlen(temp->maps[i]) - 1] = '\0';//���ص�ͼ���� �޶�����initgrah��ӡ��񣨴��ڴ�С��
        for (int j = 0; temp->maps[i][j] != '\0'; j++) {
            switch (temp->maps[i][j]) {
                case 'X':
                case 'Q':                           //mΪ�� nΪ��
                    temp->aim_m[temp->aim_sum] = i;
                    temp->aim_n[temp->aim_sum] = j;
                    temp->aim_sum++;
                    break;
                case '@':
                    temp->m = i;
                    temp->n = j;
                    break;
            }
        }
        if (temp->maps[i][1] == '|') {
            temp->maps[i][1] = '=';
            break;
        }
    }
    fclose(fp);
    return temp;
}

FILE *openfile(char *route, const char *mode)//���ļ�ָ�뺯������Ϊ ����Ƿ�ָ��Ϊ��(�Ƿ���ڹؿ��ļ����˵��ļ�) �����ش��������Ӧ��ָ��
{
    FILE *temp = fopen(route, mode);
    if (temp == NULL) {
        printf("wrong");
        getchar();
        exit(1);
    }

    return temp;
}

Interfs *interf(int scene)//�˵����溯�� ͨ������scene����sceneֵ�ı仯ʵ�ֽ�������ת
{
    // scene=1 �����ӽ���
    //scene=2 ��ʼ����
    //scene=3 ѡ�ؽ���
    //scene=4 ���ؽ���
    //scene=5 ͨ�ؽ���
    //scene=6 �ڹؿ������˳�
    Interfs *temp = &Interf;
    char game_f[256];
    sprintf(game_f, "../Interface/%d.txt", scene);
    FILE *fi;
    fi = openfile(game_f, "r");
    int line;
    fscanf(fi, "%2d", &line);
    for (int i = 0; i < line; i++) {
        fgets(temp->game_interf[i], 50, fi);
        temp->game_interf[i][strlen(temp->game_interf[i]) - 1] = '\0';
    }
    fclose(fi);
    if (scene == 1) {



        /*for (int i = 0; i < 15; i++)
        {

            puts(temp->game_interf[i]);
        }
        Sleep(1500);
        getchar();
        system("cls");*/
        IMAGE Img;

        loadimage(&Img, "../mario.jpg");
        initgraph(1200, 600);
        putimage(0, 0, &Img);
        change();
        temp = NULL;
        return temp;
    } else if (scene == 2) {

        IMAGE IMG;

        loadimage(&IMG, "../VER2.jpg");
        initgraph(1440, 900);
        putimage(0, 0, &IMG);
        change();
        temp = NULL;
        return temp;
    }
    return temp;
}

char event() //��Ϊ�����¼����ж� ͨ����������getch�����ж����벢��������(���)��ֵ
{            //���˸������� ��֤��Ч�ļ������������

    char option = getch();

    option = tolower(option);//�ϲ���Сд��ĸ

    switch (option) {

        case 'w':

            return 'w';
        case 's':
            return 's';
        case 'a':
            return 'a';
        case 'd':
            return 'd';
        case 'c':
            return 'c';
        case 'p':
            return 'p';
        case 'm':
            return 'm';
        case 'r':
            return 'r';
        case 'n':
            return 'n';
        case 'b':
            return 'b';
        case 'u':
            return 'u';
        case 13:
            return 'q';
        case 27:
            return 'e';
        default:
            option = getch();//���С���̵����� ����getch()�ɷ���С����ʵ�ʵ���ֵ ������Ӧ�������ĸ���ش�ɺϲ�ͳһ
            switch (option) {
                case 72:
                    return 'w';
                case 80:
                    return 's';
                case 75:
                    return 'a';
                case 77:
                    return 'd';
                default:
                    break;

            }
    }
}

void update()//ʵʱ���¼������� ����������ѡ�����Ч��
{
    char option = event(); //��ȡ��������ķ���ֵ
    system("cls");//ˢ������

    switch (option)//�ڼ��̵������� �����ӷ�����Ӧ���� �����������Ӧ�Ľ��
    {

        case 'w':
            record(map, step);

            switch (temp->maps[y - 1][x]) {
                case ' ':
                case 'X':

                    temp->maps[y][x] = ' ';
                    temp->maps[y - 1][x] = '@';
                    y--;
                    step++;
                    break;
                case 'O':
                case 'Q':
                    switch (temp->maps[y - 2][x]) {
                        case ' ':
                            temp->maps[y][x] = ' ';
                            temp->maps[y - 1][x] = '@';
                            temp->maps[y - 2][x] = 'O';
                            y--;
                            step++;
                            mciSendString("close jump.mp3", 0, 0, 0);
                            jump();
                            break;
                        case 'X':
                            temp->maps[y][x] = ' ';
                            temp->maps[y - 1][x] = '@';
                            temp->maps[y - 2][x] = 'Q';
                            y--;
                            step++;
                            mciSendString("close love.mp3", 0, 0, 0);
                            complete();
                            break;
                        default:
                            break;

                    }
            }
            break;
        case 's':
            record(map, step);
            switch (temp->maps[y + 1][x]) {
                case ' ':
                case 'X':
                    temp->maps[y][x] = ' ';
                    temp->maps[y + 1][x] = '@';
                    y++;
                    step++;
                    break;
                case 'O':
                case 'Q':
                    switch (temp->maps[y + 2][x]) {
                        case ' ':
                            temp->maps[y][x] = ' ';
                            temp->maps[y + 1][x] = '@';
                            temp->maps[y + 2][x] = 'O';
                            y++;
                            step++;
                            mciSendString("close jump.mp3", 0, 0, 0);
                            jump();
                            break;
                        case 'X':
                            temp->maps[y][x] = ' ';
                            temp->maps[y + 1][x] = '@';
                            temp->maps[y + 2][x] = 'Q';
                            y++;
                            step++;
                            mciSendString("close love.mp3", 0, 0, 0);
                            complete();
                            break;
                        default:
                            break;
                    }
            }
            break;
        case 'a':
            record(map, step);
            switch (temp->maps[y][x - 1]) {
                case ' ':
                    temp->maps[y][x] = ' ';
                    temp->maps[y][x - 1] = '@';
                    x--;
                    step++;
                    break;
                case 'X':
                    temp->maps[y][x] = ' ';
                    temp->maps[y][x - 1] = '@';
                    x--;
                    step++;
                    break;
                case 'O':
                case 'Q':
                    switch (temp->maps[y][x - 2]) {
                        case ' ':
                            temp->maps[y][x] = ' ';
                            temp->maps[y][x - 1] = '@';
                            temp->maps[y][x - 2] = 'O';
                            x--;
                            step++;
                            mciSendString("close jump.mp3", 0, 0, 0);
                            jump();
                            break;
                        case 'X':
                            temp->maps[y][x] = ' ';
                            temp->maps[y][x - 1] = '@';
                            temp->maps[y][x - 2] = 'Q';
                            x--;
                            step++;
                            mciSendString("close love.mp3", 0, 0, 0);
                            complete();
                            break;
                        default:
                            break;
                    }
            }
            break;
        case 'd':
            record(map, step);
            switch (temp->maps[y][x + 1]) {
                case ' ':
                    temp->maps[y][x] = ' ';
                    temp->maps[y][x + 1] = '@';
                    x++;
                    step++;
                    break;
                case 'X':
                    temp->maps[y][x] = ' ';
                    temp->maps[y][x + 1] = '@';
                    x++;
                    step++;
                    break;
                case 'O':
                case 'Q':
                    switch (temp->maps[y][x + 2]) {
                        case ' ':
                            temp->maps[y][x] = ' ';
                            temp->maps[y][x + 1] = '@';
                            temp->maps[y][x + 2] = 'O';
                            x++;
                            step++;
                            mciSendString("close jump.mp3", 0, 0, 0);
                            jump();
                            break;
                        case 'X':
                            temp->maps[y][x] = ' ';
                            temp->maps[y][x + 1] = '@';
                            temp->maps[y][x + 2] = 'Q';
                            x++;
                            step++;
                            mciSendString("close love.mp3", 0, 0, 0);
                            complete();
                            break;
                        default:
                            break;
                    }
            }
            break;
        case 'c':
            mciSendString("close Mario.mp3", 0, 0, 0);

            scene = 3;
            is_scene = 1;
            break;
        case 'p':
        case 'r':


            level = 1;
            initmap();

            is_scene = 0;
            initgraph(18 * 64, 15 * 64, EW_SHOWCONSOLE);
            change();
            loadImg();
            BGM();

            break;
        case 'm':

            level = 1;
            initmap();

            is_scene = 0;
            initgraph(18 * 64, 15 * 64, EW_SHOWCONSOLE);
            change();
            loadImg();
            BGM();

            break;
        case 'n':
            if (scene == 4 || scene == 6) {
                ++level;
                if (level > 35) {
                    level = 1;
                    is_scene = 1;
                    scene = 2;
                    mciSendString("close Mario.mp3", 0, 0, 0);
                    break;
                } else {

                    initmap();
                    is_scene = 0;
                    initgraph(18 * 64, 15 * 64, EW_SHOWCONSOLE);
                    change();
                    loadImg();
                    BGM();
                }
            } else;
            break;
        case 'b':
            initmap();
            is_scene = 0;
            initgraph(18 * 64, 15 * 64, EW_SHOWCONSOLE);
            change();
            loadImg();
            BGM();

            break;

        case 'e':
            if (is_scene == -1 || scene == 6)//˵����ʱ�Ѿ���ӡ��scene=6�˻����ڳ�ʼ�����˳�
            {
                mciSendString("close Mario.mp3", 0, 0, 0);
                closegraph();
                exit(1);
            } else {
                mciSendString("close Mario.mp3", 0, 0, 0);
                closegraph();
                scene = 6;
                is_scene = 1;
            }
            break;
        case 'q':
            break;
        case 'u':
            if (step > 0) {
                step--;
                Undo(step);
            }
            mciSendString("close back.mp3", 0, 0, 0);
            back();
            break;
            break;
        default:
            mciSendString("close Mario.mp3", 0, 0, 0);

            closegraph();
            scene = 2;
            is_scene = 1;
            break;
    }

    //�жϹ���
    if (is_scene != 1) {
        int fcount = 0;
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < strlen(temp->maps[i]); j++) {
                if (temp->maps[i][j] == 'Q') {

                    fcount++;
                }
            }
        }

        if (fcount == temp->aim_sum) {

            if (level != 35) {
                mciSendString("close end.mp3", 0, 0, 0);
                mciSendString("close Mario.mp3", 0, 0, 0);
                soundend();
                Sleep(1500);
                closegraph();
                scene = 4;
                is_scene = 1;
            } else {
                mciSendString("close end.mp3", 0, 0, 0);
                mciSendString("close Mario.mp3", 0, 0, 0);
                soundend();
                Sleep(1500);
                closegraph();
                scene = 5;
                is_scene = 1;
            }
        }
    }

    //��ԭX
    int x_m, x_n;
    for (int i = 0; i < temp->aim_sum; i++) {
        x_m = temp->aim_m[i];
        x_n = temp->aim_n[i];
        if (temp->maps[x_m][x_n] != '@' && temp->maps[x_m][x_n] != 'Q')
            temp->maps[x_m][x_n] = 'X';
    }
}

void render() {


    Interfs *it = &Interf;
    Maps *mp = &map;

    if (level != temp->level) {
        initmap();

    }

    if (is_scene) {
        if (scene == 3) {



            /*for (int i = 0; i < 15; i++)
            {

                puts(it->game_interf[i]);
            }
            printf("�밴���λس���ȷ��ѡ��\n");
            printf("ѡ��: ");
            scanf("%d", &level);
            fflush(stdin);
            while (level < 0 || level>35)
            {
                printf("����ؿ�����\n");
                printf("�밴���λس���ȷ��ѡ��\n");
                printf("����������:");
                scanf("%d", &level);
                fflush(stdin);
                if (level > 0 && level < 36)
                    break;
            }*/
            int temp = level;
            char select[20];

            InputBox(select, 30, _TEXT("��������Ĺؿ���,���ؿ���Ϊ35������Ϊ������Ȼ�ǵ���"), _TEXT("�ؿ���ѡ��"), _TEXT(""),
                     200, 20, false);
            level = atoi(select);

            while (1) {


                if (level == NULL) {

                    level = temp;
                    break;

                }
                if (level > 35 || level < 1) {

                    InputBox(select, 30, _TEXT("��������Ĺؿ���,���ؿ���Ϊ35,����Ϊ������Ȼ�ǵ���"), _TEXT("�ؿ���ѡ��"),
                             _TEXT(""), 200, 20, false);
                    level = atoi(select);

                } else {

                    break;

                }
            }


            initmap();
            initgraph(18 * 64, 15 * 64, EW_SHOWCONSOLE);
            change();
            loadImg();
            BGM();

        } else if (scene == 2) {
            interf(scene);


        } else if (scene == 6) {

            closegraph();

            interf(scene);
            IMAGE IMAGE;

            loadimage(&IMAGE, "../VER6.png");
            initgraph(1024, 576);
            putimage(0, 0, &IMAGE);
            change();
        } else {


            interf(scene);

            IMAGE img;

            loadimage(&img, "../LOVE.jpg");
            initgraph(1200, 1200);
            putimage(0, 0, &img);
            change();


        }
        is_scene = 0;
        if (scene == 6) {
            is_scene = -1;

        } else if (scene == 3)
            scene = 0;
    } else {

        for (int i = 0; i < 20; i++) {

            for (int k = 0; k < 20; k++) {

                int xone = k * 64;
                int yone = i * 64;

                switch (mp->maps[i][k]) {
                    case ' ':
                        putimage(xone, yone, img_all);
                        break;
                    case '=':
                        putimage(xone, yone, img_all + 6);
                        break;
                    case '#':
                        putimage(xone, yone, img_all + 1);
                        break;
                    case 'X':
                        putimage(xone, yone, img_all + 2);
                        break;
                    case 'O':
                        putimage(xone, yone, img_all + 3);
                        break;
                    case '@':
                        putimage(xone, yone, img_all + 4);
                        break;
                    case 'Q':
                        putimage(xone, yone, img_all + 5);
                        break;
                    case '|':
                        putimage(xone, yone, img_all + 6);
                        break;
                }

            }
        }


        printf("��%d��\n", level);
        printf("W:�� S:�� A:�� D:�� ����:U\n");
        printf("��������%d��\n", step);
    }

}

//��¼ʵʱλ��
void record(Maps map, int step) {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < strlen(temp->maps[i]); j++) {
            withdraw[step].r_map[i][j] = temp->maps[i][j];
        }
    }
}

//�����˲���ʵ��
void Undo(int step) {
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            temp->maps[i][j] = withdraw[step].r_map[i][j];
        }
    }
    for (int i = 0; i < 50; i++) {
        for (int j = 0; j < 50; j++) {
            if (temp->maps[i][j] == '@') {
                x = j;
                y = i;
            }
        }
    }

}
