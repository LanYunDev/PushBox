//#pragma once

#ifndef PUSHBOX_PUSHBOX_H
#define PUSHBOX_PUSHBOX_H

void Init();//初始化函数

void game();//游戏函数

Position *Draw(Position *p);//绘制函数

Position *Move(Position *head, Position *p);//移动函数

Position *moveBack(Position *head, Position *p);//回退函数

void moveUp(Position *p);//向上移动函数

void moveDown(Position *p);//向下移动函数

void moveLeft(Position *p);//向左移动函数

void moveRight(Position *p);//向右移动函数

void win();//完成一关✅函数

void game_over();//游戏结束函数

#endif // PUSHBOX_PUSHBOX_H
