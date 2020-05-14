#include "widget.h"
#include <QDebug>
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setFixedSize(WIDTH,HEIGHT);
    setWindowTitle("坦克大战");
    setWindowIcon(QIcon((rootdir+"pic//icon.png").c_str()));
        setStyleSheet("background-color:black;");
    campRect.setRect(12*BASESIZE,24*BASESIZE,SIZE,SIZE);
    //加载图像
    bg_gray.load((rootdir+"pic\\bg_gray.gif").c_str());
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

    timer1 = new QTimer(this);
    timer2 = new QTimer(this);
    timer3 = new QTimer(this);
    timer4 = new QTimer(this);
    timer5 = new QTimer(this);
    connect(timer1,&QTimer::timeout,this,&Widget::play);
    connect(timer2,&QTimer::timeout,this,&Widget::enemyMove);
    connect(timer3,&QTimer::timeout,this,&Widget::enemyShot);
    connect(timer4,&QTimer::timeout,this,&Widget::bulletMove);
    connect(timer5,&QTimer::timeout,this,&Widget::refresh);
    gate=1;
    init();
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
    if(start>0)
    {
        return;
    }
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
        else if(true==campRect.intersects(enemy.bullet.rect)||true==campRect.intersects(role1.bullet.rect))
        {
            QSound::play((rootdir+"\\sound\\player_bomb.wav").c_str());
            camp.load((rootdir+"pic\\camp1.gif").c_str());
            camp = resizePic(camp,SIZE,SIZE);
            update();
            gameOver();
        }
    }
    //玩家子弹和敌方坦克碰撞检测
    for(auto enemy=enemies.begin();enemy!=enemies.end();enemy++)
    {
        if(true==role1.bullet.rect.intersects(enemy->rect))
        {
            QSound::play((rootdir+"\\sound\\enemy-bomb.wav").c_str());
            enemies.erase(enemy);//需要重载 == 操作符
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
            enemy.bullet.setActive(false);
            createPlayer();
            QSound::play((rootdir+"\\sound\\player_bomb.wav").c_str());
            life--;
            if(life<=0)
            {
                gameOver();
            }
            break;
        }
    }
}

void Widget::nextGate()
{
    if(gate==35)
    {
        QMessageBox msg(QMessageBox::Icon(QMessageBox::Icon::Information),"Victory","恭喜你通过全部关卡");
        msg.exec();
        exit(1);
    }
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

    camp.load((rootdir+"pic\\camp0.gif").c_str());
    camp = resizePic(camp,SIZE,SIZE);

    QSound::play((rootdir+"sound\\start.wav").c_str());
    //创建敌人
    cursor=0;
    enemyNum = 20;
    for(int i=0;i<4;i++)
    {
        createEnemy();
    }
    //创建玩家
    createPlayer();
    life = 3;
    // 开始游戏
    start = 30;
    timer1->start(120);
    timer2->start(150);
    timer3->start(1000);
    timer4->start(60);
    timer5->start(33);
}

void Widget::createPlayer()
{
    role1.bullet.setActive(false);
    role1.setDir(direct::up);
    role1.ismove=false;
    role1.rect.setRect(9*BASESIZE,24*BASESIZE,SIZE,SIZE);
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
        if(NULL==(file=fopen((QCoreApplication::applicationDirPath()+"\\res\\map.dat").toStdString().c_str(),"rb")))
            throw "无法加载地图，请确保可执行文件所在目录的res目录下有map.dat文件，并且该游戏不放在中文目录下";

    }
    catch(const char *err)
    {
        QMessageBox msg(QMessageBox::Icon(QMessageBox::Icon::Critical),"ERROR",err);
        msg.exec();
        exit(-1);

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
    paint.drawPixmap(campRect.x(),campRect.y(),camp);
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
    paint.drawText(6*SIZE,BASESIZE,"第"+QString::number(gate)+"关，敌人数量："+QString::number(enemyNum));
    paint.drawText(7*SIZE,29*BASESIZE,"生命"+QString::number(life));
}

void Widget::drawStart()
{
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<15;j++)
        {
            paint.drawPixmap(i*SIZE,j*SIZE,bg_gray);
        }
    }
    paint.setFont(QFont("宋体",24));
    paint.drawText(12*BASESIZE+10,13*BASESIZE+10,"第"+QString::number(gate)+"关");
}

void Widget::paintEvent(QPaintEvent *)
{
    paint.begin(this);
    if(0<start--)
    {
        drawStart();
        paint.end();
        return;
    }
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

void Widget::gameOver()
{
    timer1->stop();
    timer2->stop();
    timer3->stop();
    timer4->stop();
    timer5->stop();
    QMessageBox msg(QMessageBox::Icon::Question,"GameOver","游戏结束是否重新开始？",(QMessageBox::StandardButton::Ok|QMessageBox::StandardButton::Cancel));
    if(QMessageBox::Ok == msg.exec())
    {
//        qApp->exit(RETCODE_RESTART);
//        enemies.clear();//线程访问出错
        for(auto it=enemies.begin();it!=enemies.end();it++)
            enemies.erase(it);
        gate=1;
        init();
    }
    else
    {
        this->close();
    }
}

