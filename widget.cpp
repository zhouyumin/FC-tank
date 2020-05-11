#include "widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(WIDTH,HEIGHT);
    setWindowTitle("坦克大战");
    setStyleSheet("background-color:black;");
    setWindowIcon(QIcon((rootdir+"pic//icon.png").c_str()));
    gate=1;
    init();
    timer1 = new QTimer(this);
    timer1->start(120);
    timer2 = new QTimer(this);
    timer2->start(150);
    timer3 = new QTimer(this);
    timer3->start(1000);
    timer4 = new QTimer(this);
    timer4->start(60);
    timer5 = new QTimer(this);
    timer5->start(33);
    connect(timer1,&QTimer::timeout,this,&Widget::play);
    connect(timer2,&QTimer::timeout,this,&Widget::enemyMove);
    connect(timer3,&QTimer::timeout,this,&Widget::enemyShot);
    connect(timer4,&QTimer::timeout,this,&Widget::bulletMove);
    connect(timer5,&QTimer::timeout,this,&Widget::refresh);
}

Widget::~Widget()
{
    delete timer1;
    delete timer2;
    delete timer3;
    delete timer4;
    delete timer5;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key::Key_W)
    {
        role1.setDir(direct::up);
    }
    else if(event->key()==Qt::Key::Key_S)
    {
        role1.setDir(direct::down);
    }
    else if(event->key()==Qt::Key::Key_A)
    {
        role1.setDir(direct::left);
    }
    else if(event->key()==Qt::Key::Key_D)
    {
        role1.setDir(direct::right);
    }
    else if(event->key()==Qt::Key::Key_J)
    {
        if(!role1.bullet.getActive())
        {
            role1.shot();
            QSound::play((rootdir+"\\sound\\shoot.wav").c_str());
        }
    }

}

void Widget::keyReleaseEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key::Key_J)
    {
        return;
    }
    else
    {
        role1.ismove=false;
    }
}

void Widget::collisionCheck()
{
    //玩家子弹和敌方子弹碰撞检测
    for(auto& enemy:enemies)
    {
        if(true==role1.bullet.rect.intersects(enemy.bullet.rect))
        {
            role1.bullet.setActive(false);
            enemy.bullet.setActive(false);
            break;
        }
    }
    //玩家子弹和敌方坦克碰撞检测
    for(auto& enemy:enemies)
    {
        if(true==role1.bullet.rect.intersects(enemy.rect))
        {
            QSound::play((rootdir+"\\sound\\enemy-bomb.wav").c_str());
            enemies.removeOne(enemy);//需要重载 == 操作符
            enemyNum--;
            if(enemyNum<=0)
            {
                return;
            }
            createEnemy();
            role1.bullet.setActive(false);
            break;
        }
    }
    //玩家和敌方子弹碰撞检测
    for(auto& enemy:enemies)
    {
        if(true==role1.rect.intersects(enemy.bullet.rect))
        {
            qDebug()<<"wsl";
        }
    }
}

void Widget::nextGate()
{
    gate++;
    init();
}

void Widget::play()
{
    //玩家移动
    if(role1.ismove)
    {
        role1.move();
        QSound::play((rootdir+"\\sound\\move.wav").c_str());
    }
}

void Widget::enemyMove()
{
    //敌方坦克移动
    static int d;
    srand((unsigned)time(NULL));
    for (auto& enemy:enemies)//使用引用类型 ，不然不是同一个对象
    {
        //随机方向移动
        d = rand()%4;
        if(d==0)
        {
            enemy.setDir(direct::left);
        }
        else if(d == 1)
        {
            enemy.setDir(direct::up);
        }
        else if(d == 2)
        {
            enemy.setDir(direct::right);
        }
        else if(d == 3)
        {
            enemy.setDir(direct::down);
        }
        enemy.move();
    }

}

void Widget::enemyShot()
{
    for(auto& enemy: enemies)
    {
        if(!enemy.bullet.getActive())
        {
            enemy.shot();
        }
    }
}

void Widget::bulletMove()
{
    //玩家子弹移动
    if(role1.bullet.getActive())
    {
        role1.bullet.move();
    }
    //敌人子弹移动
    for(auto& enemy:enemies)
    {
        if(enemy.bullet.getActive())
        {
            enemy.bullet.move();
        }
    }
}

void Widget::refresh()
{
    collisionCheck();
    if(enemyNum<=0)
    {
        nextGate();
    }
    update();
}

void Widget::init()
{
    //加载地图
    loadMap();
    //加载图像
    bg_gray.load((rootdir+"pic//bg_gray.gif").c_str());
    bg_gray=resizePic(bg_gray,SIZE,SIZE);
    grass.load((rootdir+"pic\\forest.gif").c_str());
    grass = resizePic(grass,BASESIZE,BASESIZE);
    brick.load((rootdir+"pic\\wall.gif").c_str());
    brick = resizePic(brick,BASESIZE,BASESIZE);
    iron.load((rootdir+"pic\\stone.gif").c_str());
    iron = resizePic(iron,BASESIZE,BASESIZE);
    water.load((rootdir+"pic\\river-0.gif").c_str());
    water = resizePic(water,BASESIZE,BASESIZE);
    ice.load((rootdir+"pic\\ice.gif").c_str());
    ice = resizePic(ice,BASESIZE,BASESIZE);
    camp.load((rootdir+"pic\\camp0.gif").c_str());
    camp = resizePic(camp,SIZE,SIZE);
    enemyIcon.load((rootdir+"pic\\enemytank-ico.gif").c_str());
    enemyIcon = resizePic(enemyIcon,BASESIZE,BASESIZE);

    QSound::play((rootdir+"\\sound\\start.wav").c_str());
    //创建敌人
    cursor=0;
    enemyNum = 20;
    for(int i=0;i<4;i++)
    {
        createEnemy();
    }
    //创建玩家
    createPlayer();
}

void Widget::createPlayer()
{
    role1.bullet.setActive(false);
    role1.rect.setRect(9*BASESIZE,24*BASESIZE,SIZE,SIZE);
    life = 3;
}

void Widget::createEnemy()
{
    if(enemyNum<4)
        return;
    Enemy enemy;
    enemy.rect.setRect(cursor*SIZE,0,SIZE,SIZE);
    enemies.push_back(enemy);
    cursor+=6;
    cursor%=18;
}

void Widget::loadMap()
{
    FILE *file;
    try
    {
        if(NULL==(file=fopen((rootdir+"data\\map.dat").c_str(),"rb")))
            throw "can not open map.dat";
    }
    catch(const char *err)
    {
        qDebug()<<err;
    }
    fseek(file,sizeof(map)*(gate-1),SEEK_SET);
    fread(&map,sizeof(map),1,file);
    fclose(file);
    file=nullptr;
}

void Widget::drawMap()
{
    for (int i=0;i<26;i++)
    {
        for(int j=0;j<26;j++)
        {
            if(map[i][j]=='3')
            {
                paint.drawPixmap(j*BASESIZE,i*BASESIZE,brick);
            }
            else if(map[i][j]=='5')
            {
                paint.drawPixmap(j*BASESIZE,i*BASESIZE,iron);
            }
            else if(map[i][j]=='1')
            {
                paint.drawPixmap(j*BASESIZE,i*BASESIZE,grass);
            }
            else if(map[i][j]=='4')
            {
                paint.drawPixmap(j*BASESIZE,i*BASESIZE,water);
            }
            else if(map[i][j]=='2')
            {
                paint.drawPixmap(j*BASESIZE,i*BASESIZE,ice);
            }
        }
    }
    paint.drawPixmap(12*BASESIZE,24*BASESIZE,camp);
}

void Widget::drawFrame()
{
    //画边框
    for(int i=0;i<15;i++)
    {
        paint.drawPixmap(0,i*SIZE,bg_gray);
        paint.drawPixmap(14*SIZE,i*SIZE,bg_gray);
        paint.drawPixmap(i*SIZE,0,bg_gray);
        paint.drawPixmap(i*SIZE,14*SIZE,bg_gray);
    }
}

void Widget::drawPanel()
{
    paint.drawText(6*SIZE,BASESIZE,"敌人数量："+QString::number(enemyNum));
    paint.drawText(7*SIZE,29*BASESIZE,"生命"+QString::number(life));
}

void Widget::paintEvent(QPaintEvent *)
{
    paint.begin(this);

    //转换坐标系统
    paint.save();
    paint.translate(SIZE,SIZE);

    //画地图
    drawMap();

    //画玩家坦克
    static bool state=true;
    role1.display(paint,state);
    state = !state;
    //画玩家子弹
    role1.bullet.display(paint);
    if(role1.bullet.bump)
    {
        role1.bullet.showExplosion(paint);
    }
    // 画敌人
    for(auto& enemy:enemies)
    {
        //画子弹
        enemy.bullet.display(paint);
        if(enemy.bullet.bump)
        {
            enemy.bullet.showExplosion(paint);
        }
        //画坦克
        enemy.display(paint,state);
    }

    //重置坐标系统
    paint.restore();

    //画边框
    drawFrame();
    //画信息面板
    drawPanel();

    paint.end();

}

