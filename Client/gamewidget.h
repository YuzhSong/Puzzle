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
#include <QTemporaryDir>
#include <QFont>
#include "settingpage.h"
#include "client.h"
#define TO_BOMB 100
#define SPECIAL_MASK 1000  // 特殊宝石的掩码

// 特殊宝石类型
#define BOMB_GEM_TYPE 1    // 爆炸宝石
#define CROSS_GEM_TYPE 2   // 十字消宝石
#define UNIVERSAL_GEM_TYPE 3  // 万能宝石
#define COLLECT_GEM_TYPE 4    // 收集宝石
extern Client *client;

class Point {
public:
    int x;
    int y;
    Point() : x(-1), y(-1) {}
    Point(int x1, int y1) : x(x1), y(y1) {}
};

namespace Ui {
class GameWidget;
}

class GameWidget : public QMainWindow
{
    Q_OBJECT

public:
    int DIFFICULITY;
    explicit GameWidget(QWidget *parent = nullptr);
    void setAdaptedImg(QString path, QLabel *label);
    void setupScene(int i);
    QSoundEffect* sound;

    // 添加用户信息设置函数
    void setUserInfo(QString username, int difficulty);

protected:
    virtual void keyPressEvent(QKeyEvent *ev) override;

private:
    // 成员变量
    bool gameOver = false;
    bool is_acting = false;
    bool is_paused = false;
    bool exitMagic = false;
    int FTime = 0;
    int mousePosX = -1, mousePosY = -1;
    int hintArrowTimes = 6;
    int score = 0;
    int eliminateTimes = 0;
    int fallNum = 0;
    int fallCount = 0;
    int selectedX = -1, selectedY = -1;
    double trans = 0;
    int redBordershow = 0;

    // 用户信息
    QString currentUsername;
    int currentDifficulty;

    // 提示功能相关
    bool hintBlinkState = false;
    QTimer* hintBlinkTimer = nullptr;
    QLabel* hintLabel1 = nullptr;
    QLabel* hintLabel2 = nullptr;
    QTimer* timeoutTimer = nullptr;
    QTimer* redBorderTimer = nullptr;
    QLabel* outLabel = nullptr;
    QLabel* redBorder = nullptr;

    std::pair<Point, Point> hintSwapPair;

    QSoundEffect *effect = nullptr;
    QLabel* scoreTextLbl = nullptr;
    QLabel* pauseBKLbl = nullptr;
    QMovie* pauseBKgif = nullptr;
    QLabel* pauseTXLbl = nullptr;

    unsigned int gemType[8][8];
    Gem* gems[8][8];
    int fallHeight[8][8];
    int tHeight[8][8];

    std::vector<Gem*> bombList;
    std::vector<Gem*> bombsToMakeMagic1;
    std::vector<Gem*> bombsToMakeMagic2;
    std::vector<Gem*> tList1;
    std::vector<Gem*> tList2;

    QWidget* boardWidget = nullptr;
    HoverButton *reSetButton = nullptr;
    HoverButton *menuButton = nullptr;
    HoverButton *hintButton = nullptr;
    HoverButton *pauseButton = nullptr;
    MyProBar* progressBar = nullptr;
    QTimer *progressTimer = nullptr;
    QLabel* selectedLbl = nullptr;

    settingpage settingP;

    // 函数声明 - 添加缺失的声明
    unsigned int getBaseType(unsigned int type);
    bool typesMatch(unsigned int type1, unsigned int type2);

    // 修正函数声明，添加引用符号 &
    int getBombsToMakeMagic(int cX, int cY, std::vector<Gem*>& bombsToMakeMagic, int time);

    QPropertyAnimation* startfallAnimation(Gem *gem, int h);
    int randomGem();
    void startGame();
    void fall();
    void fill();
    void magicFall();
    void magicFill();
    void makeStopSpin(int, int);
    void makeSpin(int, int);
    void swap(int, int, int gemX, int gemY);
    void eliminateBoard();
    Point tipsdetect();
    void fallAnimation(Gem *gem, int h);
    void fillfallAnimation(Gem *gem, int h);
    void Sleep(int msec);
    void act(Gem* gem);
    void playSound(int type);
    void forbidAll(bool forbid);
    void showHint();
    bool hasMatch(int x1, int y1, int x2, int y2);
    void generateMagic(int cX, int cY, int type, int time);
    void finishAct();
    void magicCollect(int coType, int toX, int toY);
    void allFallOut();
    void reSetBoard();
    int calculateComboScore(int rawScore, int combo);

    // 游戏结束处理
    void handleGameEnd();

    // 提示功能相关函数
    void clearHintHighlight();
    std::pair<Point, Point> findSwapHint();
    bool checkThreeMatch(int x1, int y1, int x2, int y2);

    bool tryGenerateSpecial(int x1, int y1, int x2, int y2);
    int updateBombList();
    // 新增辅助函数
    QString getSpecialBorderStyle(int type);
    unsigned int getSpecialType(unsigned int type);
    int getSpecialGemType(int x, int y);

    bool checkGemMatch(int x1, int y1, int x2, int y2);
    // 添加新的函数声明
    bool checkSpecialGemCondition(int cX, int cY, int& specialType, std::vector<Gem*>& adjacentGems);
    bool hasSimpleMatch(); // 检测是否有简单的三消
    Ui::GameWidget *ui;

signals:
    void showStartPage();
    void eliminateFinished();
    void myMouseMove(QMouseEvent*);
    void finishCount();
};

#endif // GAMEWIDGET_H
