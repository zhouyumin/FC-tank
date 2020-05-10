#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include "config.h"
#include <QIcon>
#include <QTimer>
#include "player.h"
#include <QSound>
#include <QList>
#include <enemy.h>

class Widget : public QWidget
{
    Q_OBJECT
private:
    QPixmap grass;
    QPixmap brick;
    QPixmap water;
    QPixmap ice;
    QPixmap iron;
    QPixmap bg_gray;
    QPixmap camp;
    void init();

    player role1;
    QPainter paint;
    QTimer* timer;
    QSound* start;
    QSound* shoot;
    QSound* moveSound;
    int cursor;
    QList<Enemy> enemies;
    int enemyNum;
    int gate;
    void createEnemy();
protected:
    void loadMap();
    void drawMap();
    void drawFrame();
    void paintEvent(QPaintEvent *) override;
public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void keyPressEvent(QKeyEvent *) override;
    void keyReleaseEvent(QKeyEvent *event) override;
    void collisionCheck();
    void nextGate();
public slots:
    void play();
};
#endif // WIDGET_H
