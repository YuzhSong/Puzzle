#ifndef GAMEWIDGET_H
#define GAMEWIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QLabel>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QIcon>
#include <QTimer>
#include <hoverbutton.h>
#include <myprobar.h>
#include <QRandomGenerator>
#include "gem.h"
#include <QTime>
#include <QMainWindow>
#include <QSoundEffect>
#include<QTemporaryDir>
#include <QFont>
#include"settingpage.h"
#include "client.h"
#define TO_BOMB 100

extern Client *client;

class Point{
public:
    int x;
    int y;
    Point() : x(-1), y(-1) {}  // 添加默认构造函数
    Point(int x1, int y1) : x(x1), y(y1) {}  // 使用初始化列表
};

namespace Ui {
class GameWidget;
}

class GameWidget : public QMainWindow
{
    Q_OBJECT

public:
    int DIFFICULITY=5;
    explicit GameWidget(QWidget *parent = nullptr);
    void setAdaptedImg(QString path,QLabel *label);
    void setupScene(int i);
    QSoundEffect* sound;

protected:
    virtual void keyPressEvent(QKeyEvent *ev)override;

private:
    // 成员变量按声明顺序排列
    bool gameOver=false;
    bool is_acting=false;
    bool is_paused=false;
    bool exitMagic=false;
    int FTime=0;
    int mousePosX=-1,mousePosY=-1;
    int hintArrowTimes=6;
    int score=0;
    int eliminateTimes=0;
    int fallNum=0;
    int fallCount=0;
    int selectedX=-1,selectedY=-1;
    double trans=0;
    int redBordershow=0;

    // 提示功能相关
    bool hintBlinkState = false;       // 闪烁状态
    QTimer* hintBlinkTimer = nullptr;  // 闪烁定时器
    QLabel* hintLabel1 = nullptr;      // 第一个提示高亮框
    QLabel* hintLabel2 = nullptr;      // 第二个提示高亮框
    QTimer* timeoutTimer=nullptr;
    QTimer* redBorderTimer=nullptr;
    QLabel* outLabel=nullptr;
    QLabel* redBorder=nullptr;

    std::pair<Point, Point> hintSwapPair; // 存储可交换的两个宝石位置

    QSoundEffect *effect=nullptr;
    QLabel* scoreTextLbl=nullptr;
    QLabel* pauseBKLbl = nullptr;
    QMovie* pauseBKgif = nullptr;
    QLabel* pauseTXLbl = nullptr;

    unsigned int gemType[8][8];//type数组
    Gem* gems[8][8];//gem对象数组
    int fallHeight[8][8];//掉落高度数组
    int tHeight[8][8];

    std::vector<Gem*> bombList; //存要消去的宝石
    std::vector<Gem*> bombsToMakeMagic1;
    std::vector<Gem*> bombsToMakeMagic2;
    std::vector<Gem*> tList1;
    std::vector<Gem*> tList2;

    QWidget* boardWidget=nullptr;
    HoverButton *reSetButton=nullptr;
    HoverButton *menuButton=nullptr;
    HoverButton *hintButton=nullptr;
    HoverButton *pauseButton=nullptr;
    MyProBar* progressBar=nullptr;
    QTimer *progressTimer=nullptr;
    QLabel* selectedLbl=nullptr;

    settingpage settingP;

    // 函数声明
    QPropertyAnimation* startfallAnimation(Gem *gem, int h);
    int randomGem();
    void startGame();
    void fall();
    void fill();
    void magicFall();
    void magicFill();
    void makeStopSpin(int,int);
    void makeSpin(int,int);
    void swap(int, int, int gemX,int gemY);
    void eliminateBoard();
    Point tipsdetect();
    void fallAnimation(Gem *gem, int h);
    void fillfallAnimation(Gem *gem, int h);
    void Sleep(int msec);
    void act(Gem* gem);
    void playSound(int type);
    void forbidAll(bool forbid);
    int getBombsToMakeMagic(int cX,int cY,std::vector<Gem*> bombsToMakeMagic,int time);
    void generateMagic(int cX,int cY,int type,int time);
    void finishAct();
    void magicCollect(int coType,int toX,int toY);
    void allFallOut();
    void reSetBoard();

    // 新添加的函数声明 - 提示功能相关
    void clearHintHighlight();
    std::pair<Point, Point> findSwapHint();
    bool checkThreeMatch(int x1, int y1, int x2, int y2);

    int updateBombList();

    Ui::GameWidget *ui;

signals:
    void showStartPage();
    void eliminateFinished();
    void myMouseMove(QMouseEvent*);
    void finishCount();
};

#endif // GAMEWIDGET_H
