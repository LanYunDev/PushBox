#include "widget.h"

QString _mapFilePath = "C:/Qt/Qt5.2.0/Tools/QtCreator/bin/PushBox3D/res/map.txt";
//QString _mapFilePath = "./res/map.txt";

Widget::Widget(QWidget *parent) :
        QGLWidget(parent) {
    setWindowTitle("PushBox 3D - 推箱子1.0 - 刘志浩2014012662(NWAFU)");

    offset[0] = QPoint(1, 0);  //右
    offset[1] = QPoint(0, 1);  //下
    offset[2] = QPoint(-1, 0); //左
    offset[3] = QPoint(0, -1); //上

    _verticalAngle = 45.0f;
    _horizonAngle = 90.0f;
    _scale = 1.0f;
    sec = 0.0f;

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(showShortestPath()));
    timer->setInterval(10);
}

void Widget::readMap(QString filename) {
    QFile f(filename);

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << filename + " : Open failed";
        exit(0);
    }

    QTextStream inStream(&f);

    width = inStream.readLine().toInt();
    height = inStream.readLine().toInt();

    map = new int[width * height];
    pathMap = new int[width * height];

    QString strMap = inStream.readAll();
    QStringList strList = strMap.split(QRegularExpression("\\s+"));

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            map[(i * width) + k] = strList.at((i * width) + k).toInt();
            pathMap[(i * width) + k] = -1;
        }
    }

    // 至此, 地图数据已经读取完毕
    ground = new Plane();
    ground->setXY(width - 1, height - 1);
    ground->setSize(width * 2 + 3, height * 2 + 3);
    ground->setTexture(groundTexture);

    for (int i = 0; i < height; i++) {
        for (int k = 0; k < width; k++) {
            if (map[i * width + k] == 1) {
                Cube *wall = new Cube();
                wall->setXY(k, i);
                wall->setTexture(wallTexture);
                walls.push_back(wall);
            }

            if (map[i * width + k] == 2) {
                player = new Cube();
                player->setXY(k, i);

                _playerPos = QPoint(k, i);
                startPlayer = QPoint(k, i);
            }
            if (map[i * width + k] == 3) {
                box = new Cube();
                box->setXY(k, i);
                box->setTexture(boxTexture);
                _boxPos = QPoint(k, i);
                startBox = QPoint(k, i);
            }
            if (map[i * width + k] == 4) {
                dest = new Cube();
                dest->setRGBA(1.0, 0.0, 0.0, 0.5);
                dest->setXY(k, i);
                endBox = QPoint(k, i);
            }
        }
    }

    getShortestPath();   // 获取最短路径
}

bool Widget::canMovePlayerTo(int x, int y, int dir)  /// (x,y)为玩家的目标位置，若能推到箱子，意味着
{
    int index = y * width + x;
    if (x < 0 || x >= width || y < 0 || y >= height || map[index] == 1)
        return false;

    int dx = x - _boxPos.x();
    int dy = y - _boxPos.y();
    double dist = sqrt(dx * dx + dy * dy);

    if (dist <= 0.1 && !canMoveBoxTo(x + offset[dir].x(), y + offset[dir].y()))
        return false;

    return true;
}

bool Widget::canMoveBoxTo(int x, int y) {
    int index = y * width + x;
    if (x < 0 || x >= width || y < 0 || y >= height || map[index] == 1)
        return false;

    _boxPos.setX(x);
    _boxPos.setY(y);
    box->setXY(x, y);

    return true;
}

void Widget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt((35.0) * cos(PAI * _verticalAngle / 180.0) * cos(PAI * _horizonAngle / 180.0) + width,
              35.0 * sin(PAI * _verticalAngle / 180.0),
              (55.0) * cos(PAI * _verticalAngle / 180.0) * sin(PAI * _horizonAngle / 180.0) + height,
              width, 0.0, height, 0.0, 1.0, 0.0);

    glPushMatrix();
    glTranslatef(width, 0, height);
    glScalef(_scale, _scale, _scale);
    glTranslatef(-width, 0, -height);

    for (int i = 0; i < walls.size(); i++) {
        walls[i]->drawCube();
    }

    dest->drawCube();
    player->drawCube();
    box->drawCube();

    ground->drawPlane();

    glPopMatrix();
}

void Widget::keyPressEvent(QKeyEvent *event) {
    if (timer->isActive())
        return;
    QPoint nextPlayerPos;
    int dir;
    switch (event->key()) {
        case Qt::Key_Up:
        case Qt::Key_W:
            nextPlayerPos.setX(_playerPos.x());
            nextPlayerPos.setY(_playerPos.y() - 1);
            dir = 3;
            break;
        case Qt::Key_Down:
        case Qt::Key_S:
            nextPlayerPos.setX(_playerPos.x());
            nextPlayerPos.setY(_playerPos.y() + 1);
            dir = 1;
            break;
        case Qt::Key_Left:
        case Qt::Key_A:
            nextPlayerPos.setX(_playerPos.x() - 1);
            nextPlayerPos.setY(_playerPos.y());
            dir = 2;
            break;
        case Qt::Key_Right:
        case Qt::Key_D:
            nextPlayerPos.setX(_playerPos.x() + 1);
            nextPlayerPos.setY(_playerPos.y());
            dir = 0;
            break;
        case Qt::Key_K:
            timer->start();
            return;
        default:
            return;
    }

    if (!canMovePlayerTo(nextPlayerPos.x(), nextPlayerPos.y(), dir))
        return;

    _playerPos = nextPlayerPos;

    player->setXY(_playerPos.x(), _playerPos.y());
    updateGL();

    if (map[_boxPos.y() * width + _boxPos.x()] == 4) {
        QMessageBox::information(NULL, "Congratulations", "你赢了!  _liuzhihao");
    }
}

void Widget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        _laterPos = mapFromGlobal(event->pos());
    }
}

void Widget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        QPoint pos = mapFromGlobal(event->pos());

        int dx = pos.x() - _laterPos.x();
        int dy = pos.y() - _laterPos.y();

        if (fabs(dx) < 4.0 && fabs(dy) < 4.0)
            return;

        _horizonAngle += dx / 10.0;
        if (_verticalAngle + dy < 89 && _verticalAngle + dy > -89)
            _verticalAngle += dy / 10.0;

        _laterPos = pos;
        updateGL();
    }
}

void Widget::wheelEvent(QWheelEvent *event) {
    if (event->delta() > 0)
        _scale += 0.05;
    else if (event->delta() < 0)
        _scale -= 0.05;

    updateGL();
}

void Widget::initializeGL() {
    glShadeModel(GL_SMOOTH);

    glClearColor(0.0, 0.0, 0.0, 0.0);

    //set light
    GLfloat mat_specular[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat mat_shiniess[] = {0.0};
    GLfloat light_ambient[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_diffuse[4] = {1.0, 1.0, 1.0, 1.0};
    GLfloat light_specular[4] = {0.0, 0.0, 0.0, 1.0};
    GLfloat light_position[4] = {1.0, 1.0, 1.0, 0.0};

    GLfloat lmodel_ambient[] = {0.1, 0.1, 0.1, 1.0};

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shiniess);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glEnable(GL_COLOR_MATERIAL);

    loadTexture(":/new/res/box.jpg", &boxTexture);
    loadTexture(":/new/res/wall.png", &wallTexture);
    loadTexture(":/new/res/ground.jpg", &groundTexture);

    readMap(_mapFilePath);
}

void Widget::resizeGL(int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLdouble) w / (GLdouble) h, 0.1, 10000.0);
}

void Widget::loadTexture(QString filepath, GLuint *texture) {
    QImage tex, buf;

    if (!buf.load(filepath)) {
        qDebug() << "error: failed to load image!";
        exit(0);
    }

    tex = convertToGLFormat(buf);
    glGenTextures(1, texture);

    glBindTexture(GL_TEXTURE_2D, *texture);
    glTexImage2D(GL_TEXTURE_2D, 0, 3, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

bool Widget::isOutOfBound(QPoint p) {
    if (p.x() < 0 || p.x() >= width || p.y() < 0 || p.y() >= height)
        return true;
    return false;
}

bool Widget::canArrived(Node _cur, Node _next) {
    QPoint cur = _cur.playerPos;   // cur: 玩家当前位置

    QPoint dist = _next.playerPos; // dist：玩家的目标位置
    QPoint nbr;

    if (isOutOfBound(dist)) {
        return false;
    }

    if (map[dist.y() * width + dist.x()] == 1) {
        return false;
    }

    int tPathMap[width * height];
    for (int i = 0; i < width * height; i++)
        tPathMap[i] = -1;

    tPathMap[cur.y() * width + cur.x()] = 0;

    QQueue <QPoint> que;
    do {
        for (int i = 0; i < 4; i++) {
            nbr.setX(cur.x() + offset[i].x());  // nbr的pos为cur向四个方向探索的情况
            nbr.setY(cur.y() + offset[i].y());

            if (isOutOfBound(nbr))
                continue;

            if (tPathMap[nbr.y() * width + nbr.x()] == -1 && map[nbr.y() * width + nbr.x()] != 1 &&
                (cur.x() != _cur.pos.x() || cur.y() != _cur.pos.y())) {
                tPathMap[nbr.y() * width + nbr.x()] = tPathMap[cur.y() * width + cur.x()] + 1;
                que.push_back(nbr);
            }
            if (nbr.x() == _next.pos.x() || nbr.y() == _next.pos.y())
                return true;
        }

        if (que.isEmpty())  // 如果扩展结点用完了,则到无解.
            return false;
        cur = que.front();
        que.pop_front();
    } while (true);
}

bool Widget::getShortestPath() {
    if (map == NULL)
        return false;

    Node cur, nbr;               // cur记录当前扩展方格，nbr记录扩展方向的方格
    cur.pos = startBox;         // 初始cur.pos()为箱子初始位置
    cur.playerPos = startPlayer;  // 初始cur.playerPos为玩家初始位置
    pathMap[cur.pos.y() * width + cur.pos.x()] = 0;

    QQueue <Node> que;
    do {
        for (int i = 0; i < 4; i++) {
            nbr.pos.setX(cur.pos.x() + offset[i].x());  // nbr的pos为cur向四个方向探索的情况
            nbr.pos.setY(cur.pos.y() + offset[i].y());

            nbr.playerPos.setX(cur.pos.x() + offset[(i + 2) % 4].x());
            nbr.playerPos.setY(cur.pos.y() + offset[(i + 2) % 4].y());

            if (isOutOfBound(nbr.pos))
                continue;

            if (pathMap[nbr.pos.y() * width + nbr.pos.x()] == -1 && map[nbr.pos.y() * width + nbr.pos.x()] != 1 &&
                canArrived(cur, nbr))  // 箱子未走 && 下个位置不为墙 && 人能到达
            {
                nbr.playerPos = cur.pos;
                pathMap[nbr.pos.y() * width + nbr.pos.x()] = pathMap[cur.pos.y() * width + cur.pos.x()] + 1;
                que.push_back(nbr);
            }
            if (map[nbr.pos.y() * width + nbr.pos.x()] == 4 &&
                canArrived(cur, nbr)) // 到达终点(此处解决了一处Bug：若不加canArrived(),终点为最后一个判断点时会出现bug)
                break;
        }
        if (que.isEmpty())  // 如果扩展结点用完了,则到无解.
        {
            return false;
        }
        if (map[nbr.pos.y() * width + nbr.pos.x()] == 4)  //到达终点 退出循环
            break;
        cur = que.front();
        que.pop_front();
    } while (true);

    // 逆向构造最短布线方案
    int PathLen = pathMap[endBox.y() * width + endBox.x()];
    cur.pos = endBox;

    for (int j = PathLen - 1; j >= 0; j--) {
        shortestPath.push_front(cur.pos);
        for (int i = 0; i < 4; i++) {
            nbr.pos.setX(cur.pos.x() + offset[i].x());
            nbr.pos.setY(cur.pos.y() + offset[i].y());
            if (pathMap[nbr.pos.y() * width + nbr.pos.x()] == j)
                break;
        }
        cur = nbr;
    }
    shortestPath.push_front(startBox);

    qDebug() << "该地图路径：";
    for (int i = 0; i < shortestPath.size(); i++) {
        qDebug() << QString("(%1,%2)").arg(shortestPath[i].y()).arg(shortestPath[i].x());
    }

    return true;
}

void Widget::showShortestPath() {
    sec += 0.02;
    interpolation();
}

void Widget::interpolation() {
    int pre = (int) sec;
    int next = pre + 1;
    float alpha = sec - pre;

    if (pre >= shortestPath.size() - 1) {
        QMessageBox::information(NULL, "提示", "已经到达终点");
        sec = 0.0f;
        box->setXY(_boxPos.x(), _boxPos.y());
        timer->stop();
        return;
    }

    QPoint prePos = shortestPath[pre];
    QPoint nextPos = shortestPath[next];

    QPointF interp;
    interp.setX((1 - alpha * alpha * alpha) * prePos.x() + alpha * alpha * alpha * nextPos.x());
    interp.setY((1 - alpha * alpha * alpha) * prePos.y() + alpha * alpha * alpha * nextPos.y());

    box->setPos(interp.x() * 2, interp.y() * 2);
    updateGL();
}

Widget::~Widget() {}
