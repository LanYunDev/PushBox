#include "cube.h"

Cube::Cube() {
    width = 1.0f;
    x = z = 0.0f;
    texture = NULL;

    r = g = b = a = 1.0f;
}

void Cube::setRGBA(GLfloat _r, GLfloat _g, GLfloat _b, GLfloat _a) {
    r = _r;
    g = _g;
    b = _b;
    a = _a;
}

void Cube::setXY(int _x, int _y) {
    mx = _x;
    my = _y;

    x = mx * 2;
    z = my * 2;
}


void Cube::setWidth(GLfloat _width) {
    width = _width;
}

void Cube::setPos(GLfloat _x, GLfloat _z) {
    x = _x;
    z = _z;
}

void Cube::setTexture(GLuint _texture) {
    texture = _texture;
}

void Cube::drawCube() {
    glBindTexture(GL_TEXTURE_2D, texture);
    glPushMatrix();
    glColor4f(r, g, b, a);

    glTranslatef(x, 0, z);

    glBegin(GL_QUADS);
    glNormal3f(0.0, 1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-width, width, -width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-width, width, width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(width, width, width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(width, width, -width);
    //下顶面
    glNormal3f(0.0, -1.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-width, -width, -width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-width, -width, width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(width, -width, width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(width, -width, -width);
    //正前面
    glNormal3f(0.0, 0.0, 1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-width, width, width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-width, -width, width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(width, -width, width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(width, width, width);
    //右侧面
    glNormal3f(1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(width, width, width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(width, -width, width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(width, -width, -width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(width, width, -width);
    //背后面
    glNormal3f(0.0, 0.0, -1.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-width, width, -width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(width, width, -width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(width, -width, -width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-width, -width, -width);
    //左侧面
    glNormal3f(-1.0, 0.0, 0.0);
    glTexCoord2f(0.0, 1.0);
    glVertex3f(-width, width, -width);
    glTexCoord2f(0.0, 0.0);
    glVertex3f(-width, -width, -width);
    glTexCoord2f(1.0, 0.0);
    glVertex3f(-width, -width, width);
    glTexCoord2f(1.0, 1.0);
    glVertex3f(-width, width, width);
    glEnd();

    glPopMatrix();
}

