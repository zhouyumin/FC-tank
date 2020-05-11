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
    QPixmap enemyIcon;
    QPixmap playerIcon;
    void init();
    void createPlayer();
    player role1;
    QPainter paint;
    QTimer* timer1;
    QTimer* timer2;
    QTimer* timer3;
    QTimer* timer4;
    QTimer* timer5;
    int cursor;
    QList<Enemy> enemies;
    int enemyNum;
    int life;
    int gate;
    void createEnemy();
protected:
    void loadMap();
    void drawMap();
    void drawFrame();
    void drawPanel();
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
    void enemyMove();
    void enemyShot();
    void bulletMove();
    void refresh();
};
#endif // WIDGET_H
