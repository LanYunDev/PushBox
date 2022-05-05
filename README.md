## 前言

这个是我学校工程实践的作业.全程采用Clion编写.如果你遇到一些坑,可以参考一下我的CMakeLists.txt文件或者在尝试我博客中寻找解决办法.

另外本[项目](https://github.com/LanYunDev/PushBox)开源啦~欢迎👏点个Star~

写的不是很好(不过大部分代码行或块都有注释),欢迎👏提Issues和PR(Pull requests).

欢迎到我[博客](https://lanyundev.vercel.app)上去玩耍噢~ 博客上也有推箱子小游戏噢~

对于代码内容关于ncurses.h库那些函数不太明白可以看看我写的[这篇文章](https://lanyundev.vercel.app/posts/a5945d21.html)噢

## 功能

目前实现的功能有:

1. 基于链表的有限步数下的单次回滚(撤回)等操作
2. 基于ncurses.h库而实现的窗口和方向键等功能
3. 在玩过的关卡中可自由选择关卡
4. 通过文件读写方式存数据和地图
5. 支持数据文件丢失后的自动重置
6. 支持数据文件是否合法等的检查
7. 有通过关卡到下一关的提示界面
8. 有结束游戏开始保存数据的界面
9. 等等…

## 如何运行?

前提:

环境中有ncurses.h库,貌似不支持Windows,支持Unix和Linux.

在`main.c`目录下输入命令:

```bash
gcc main.c -o PushBox -lncurses
./PushBox
```

然后就可以快乐地玩耍啦~

游戏🎮游玩示例GIF图:

![2022-05-05 12.48.32](README/2022-05-05%2012.48.32.gif)

## ~~Todo~~

鸽了,欢迎大佬补充然后做

1. 自动寻路算法

2. 无限回滚操作
3. 自动生成地图
4. 修复fscanf安全🔐问题
5. 等等…

