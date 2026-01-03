#ifndef STARTPAGE_H
#define STARTPAGE_H

#include <QWidget>
#include <QLabel>
#include <QPropertyAnimation>
#include <QDebug>
#include <QImage>
#include <QFile>
#include "hoverbutton.h"
#include "gamewidget.h"
#include <QTime>
#include <QTemporaryDir>
#include <QSoundEffect>
#include<qmediaplayer.h>
#include<qsoundeffect.h>
#include"settingpage.h"
#include "selectlevel.h"

#include "ranklistpage.h"
#include "loginpage.h"
#include "registerpage.h"
namespace Ui {
class StartPage;
}

class StartPage : public QWidget
{
    Q_OBJECT

public:
    explicit StartPage(QWidget *parent = nullptr);
    ~StartPage();
    int ForGameL=1;
    GameWidget* gameWidget = new GameWidget;
    selectlevel* select=new selectlevel();

    loginPage *login = new loginPage;
    registerPage *regist = new registerPage;
    rankListPage *ranklist = new rankListPage;

    HoverButton *loginButton, *registerButton ,*logoutButton;

    QPropertyAnimation *bkAnim;
    settingpage settingP;
    QSoundEffect* sound;

protected:
    virtual void keyPressEvent(QKeyEvent *ev);
    void closeEvent(QCloseEvent *event);

private:
    Ui::StartPage *ui;
    HoverButton *startButton, *recordButton, *settingButton;

    QLabel *background; // 成员变量

    // 修改：将原来的 ShowBackground 拆分或修改逻辑
    void initBackground();          // 初始化静止背景
    QPropertyAnimation* createBkAnim(); // 创建背景动画
    QPropertyAnimation* ShowTitle();

    void SetButton();       // 设置主菜单按钮位置
    void setBkImg(QString path,QLabel *label);
    void setAdaptedImg(QString path,QLabel *label);
    void Sleep(int msec);

    // 新增：界面状态控制
    void initLoginUI();     // 显示初始登录界面
    void enterMainMenu();   // 进入主菜单（执行动画）

    QString currentUsername;
    QString currentDifficultyText;
    int currentGemCount;
};

#endif // STARTPAGE_H
