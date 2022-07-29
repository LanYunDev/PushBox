#ifndef CUBE_H
#define CUBE_H

#include <cstdlib>
#include <QImage>
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QDebug>

class Cube {
public:
    Cube();

    GLfloat width;
    GLuint texture;

    int mx, my;  //  Map中的坐标(mx,my)  ==>  OpenGL中的坐标(x,0,z)

    void setXY(int _x, int _y);

    GLfloat x, z;  // (x,0,z)
    GLfloat r, g, b, a;

    void setRGBA(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a);

    void setWidth(GLfloat _width);

    void setPos(GLfloat _x, GLfloat _z);

    void setTexture(GLuint _texture);

    void drawCube();
};

#endif // CUBE_H
