#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glaux.h>
#include <QFile>
#include <QKeyEvent>
#include <QTextStream>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QTimer>
#include <QMessageBox>
#include <QRegularExpression>
#include <QMenuBar>
#include <cmath>
#include <QQueue>
#include "plane.h"
#include "cube.h"

#define PAI 3.1415926

class Node {
public:
    QPoint pos;        // 箱子位置pos
    QPoint playerPos;  // 为把箱子推至pos位置，玩家要抵达的位置pos
};

class Widget : public QGLWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);

    ~Widget();

    /// 视角变换
    GLdouble _verticalAngle;
    GLdouble _horizonAngle;
    GLdouble _scale;

    QPoint _laterPos;  // 鼠标按下时的坐标记录

    GLuint boxTexture;
    GLuint wallTexture;
    GLuint groundTexture;
    GLuint destTexture;

    int width;
    int height;
    int *map;
    int *pathMap;

    QPoint startBox;
    QPoint endBox;
    QPoint startPlayer;

    QVector <QPoint> shortestPath;

    QPoint offset[4];

    QPoint _playerPos;
    QPoint _boxPos;

    float sec;
    QTimer *timer;

    QVector<Cube *> walls;
    Cube *player;
    Cube *box;
    Cube *dest; // 终点

    Plane *ground;

    void readMap(QString filename);

    void loadTexture(QString filepath, GLuint *texture);

    bool canMovePlayerTo(int x, int y, int dir);

    bool canMoveBoxTo(int x, int y);

    bool isOutOfBound(QPoint p);

    bool canArrived(Node _cur, Node _next);

    bool getShortestPath();

protected:
    void initializeGL();

    void resizeGL(int w, int h);

    void paintGL();

    void keyPressEvent(QKeyEvent *event);

    void mousePressEvent(QMouseEvent *event);

    void mouseMoveEvent(QMouseEvent *event);

    void wheelEvent(QWheelEvent *event);

private
    slots:
            void showShortestPath();

    void interpolation();
};

#endif // WIDGET_H
