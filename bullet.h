#ifndef BULLET_H
#define BULLET_H
#include "config.h"
#include <QPixmap>
#include <QPainter>

class Bullet
{
public:
    int speed;
    bool busy;
    int w;
    int h;
    direct dir;
    QPixmap upimg;
    QPixmap downimg;
    QPixmap leftimg;
    QPixmap rightimg;
    QRect rect;
    Bullet();
    void move();
    void setDir(direct);
    void display(QPainter&);
    bool canReachable(int,int,direct);
    ~Bullet();
};

#endif // BULLET_H
