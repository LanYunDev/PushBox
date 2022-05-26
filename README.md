## 前言 Foreword

这个是我学校工程实践的作业.作业采用C语言编写,语言标准C23,采用全程采用Clion作为我的IDE进行编写和调试.

如果你遇到一些坑,可以参考一下我的CMakeLists.txt文件或者在尝试我博客中寻找解决办法.

另外本[项目](https://github.com/LanYunDev/PushBox)开源啦~ 欢迎👏点个Star~

个人觉得本项目有很大提升空间(不过好在绝大多数代码行或块都有注释,阅读起来十分清晰.),欢迎👏提Issues和PR(Pull requests).

欢迎到我[博客](https://lanyundev.vercel.app)上去玩耍噢~ 博客上也有推箱子小游戏噢~

对于代码内容关于ncurses.h库那些函数不太明白可以看看我写的[这篇文章](https://lanyundev.vercel.app/posts/a5945d21.html)噢

## 功能 Function

目前实现的功能有:

1. 已实现在无限步数下的撤回操作

   ~~基于链表的有限步数下的单次回滚(撤回)等操作 (通俗点说,就是不支持多次撤回,撤回的前一步不能是撤回步骤)~~
2. 基于ncurses.h库而实现的窗口和方向键等功能 (本程序多次使用这个库里面的函数,建议了解并学习后使用它)
3. 在玩过的关卡中可自由选择关卡
4. 通过文件读写方式存数据和地图
5. 支持数据文件丢失后的自动重置
6. 支持数据文件是否合法等的检查
7. 有通过关卡到下一关的提示界面
8. 有结束游戏开始保存数据的界面
9. 关卡数和关卡游玩时间显示功能
10. 等等…

## 如何运行? ⚙ How to run?

命令行终端进入`PushBox`目录后运行启动脚本(极力推荐):

**启动脚本命令:**
```bash
chmod +x 启动脚本.sh && ./启动脚本.sh
```

若依然失败，请反馈相关信息以便本人能够排除问题并解决问题。

<details>
  <summary>点我查看以前写的关于运行的相关提示或说明</summary>

   > 运行前提(先决)条件👇
   >
   > > 环境中有ncurses.h库,中文字体库及其一些特殊字符.
   > > 目前已知能够在Unix等Linux类OS下运行.Windows用户请自行测试(大概率不支持).

   由于本人不太会使用CMake，所以如果没有ncurses.h库，可参考如下命令安装:

   ```bash
   sudo apt install libncurses5-dev
   ```

   或者尝试编译安装ncurses.h库:

   ```bash
   cd include/ncurses-6.3 && ./configure && make && sudo make install
   ```

   命令行终端进入`PushBox`目录后输入命令或运行启动脚本(推荐):

   ```bash
   cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release && cmake --build build --target all && mv build/PushBox src/ && cd src/ && ./PushBox
   ```

   如果没有Ninja生成器,可以去掉`-G Ninja`参数。

   ```bash
   cd src && gcc PushBox.c -o PushBox -lncurses && ./PushBox
   ```

   **启动脚本命令:**
   ```bash
   chmod +x 启动脚本.sh && ./启动脚本.sh
   ```
</details>

如果成功启动，那就可以快乐地玩耍啦~

<img src="README/image-20220519235819464.png" alt="image-20220519235819464" style="zoom:50%;" />

游戏🎮游玩示例GIF图:

![v0.0.3-2022-05-07 00.10.53](README/v0.0.3-2022-05-07%2000.10.53.gif)

## 🎉 Features

我鸽了,欢迎大佬补充然后做.

- [x] 无限回滚操作
- [ ] 自动寻路算法
- [x] 支持自动链接编译
- [ ] 自动生成地图文件
- [ ] 优化代码及其性能
- [ ] 支持3D图形化显示
- [ ] 支持Windows平台
- [ ] 等等…

