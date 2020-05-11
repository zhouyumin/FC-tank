#ifndef TANKBASE_H
#define TANKBASE_H
#include "bullet.h"
#include <QPixmap>
#include <QPainter>

class tankBase
{
public:
    QPixmap upimg1;
    QPixmap upimg2;
    QPixmap downimg1;
    QPixmap downimg2;
    QPixmap leftimg1;
    QPixmap leftimg2;
    QPixmap rightimg1;
    QPixmap rightimg2;
    int speed;
    QRect rect;
    direct dir;
    void shot();
    Bullet bullet;
    virtual void move();
    virtual void setDir(direct);
    virtual void display(QPainter&,bool);
    bool canReachable(int,int,direct);
    tankBase();
    tankBase& operator=(const tankBase&);
    tankBase(const tankBase&);
    ~tankBase();
};

#endif // TANKBASE_H
