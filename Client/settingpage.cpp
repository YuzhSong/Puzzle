#include "settingpage.h"
#include "ui_settingpage.h"
#include<qpixmap.h>
#include<qfile.h>
#include <QTemporaryDir>
#include<qdesktopservices.h>
#include<qurl.h>
#include<qpainter.h>

settingpage::settingpage(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::settingpage),
    index(1)  // 初始化语言索引为1（英语）
{
    ui->setupUi(this);
    QPixmap pixmap = QPixmap(":/picture/Settingpage/dialogbox.png").scaled(this->size());
    QPalette palette(this->palette());
    palette.setBrush(QPalette::Window, QBrush(pixmap));
    this->setPalette(palette);

    //去窗口边框
    this->setWindowFlags(Qt::FramelessWindowHint | this->windowFlags());
    //把窗口背景设置为透明
    this->setAttribute(Qt::WA_TranslucentBackground);

    //设置标签对鼠标事件穿透
    ui->label_help->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_done->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->label_web->setAttribute(Qt::WA_TransparentForMouseEvents);
    ui->LangL->setAttribute(Qt::WA_TransparentForMouseEvents);

    ui->Help->setCursor(QCursor(Qt::PointingHandCursor));
    ui->Web->setCursor(QCursor(Qt::PointingHandCursor));
    ui->Done->setCursor(QCursor(Qt::PointingHandCursor));
    ui->LangB->setCursor(QCursor(Qt::PointingHandCursor));

    ui->Help->setGeometry(140, 260, 131, 51);
    ui->Help->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)");
    connect(ui->Help, &QPushButton::pressed, [=](){
        ui->Help->setStyleSheet("border-image:url(:/picture/Settingpage/DialogButtonGlow.png)");}
            );
    connect(ui->Help, &QPushButton::released, [=](){
        ui->Help->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)"); }
            );

    ui->Web->setGeometry(460, 260, 131, 51);
    ui->Web->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)");
    connect(ui->Web, &QPushButton::pressed, [=](){
        ui->Web->setStyleSheet("border-image:url(:/picture/Settingpage/DialogButtonGlow.png)");}
            );
    connect(ui->Web, &QPushButton::released, [=](){
        ui->Web->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)"); }
            );

    ui->Done->setGeometry(300, 320, 131, 51);
    ui->Done->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)");
    connect(ui->Done, &QPushButton::pressed, [=](){
        ui->Done->setStyleSheet("border-image:url(:/picture/Settingpage/DialogButtonGlow.png)");}
            );
    connect(ui->Done, &QPushButton::released, [=](){
        ui->Done->setStyleSheet("border-image:url(:/picture/Settingpage/dialogbutton.png)"); }
            );

    // 隐藏音量亮度相关控件
    ui->label_volume->hide();
    ui->label_Bright->hide();

    // 可以添加提示标签
    QLabel *crossPlatformLabel = new QLabel(this);
    crossPlatformLabel->setText("音量亮度调节（仅Windows可用）");
    crossPlatformLabel->setGeometry(150, 140, 400, 40);
    crossPlatformLabel->setAlignment(Qt::AlignCenter);
    crossPlatformLabel->setStyleSheet("QLabel{color:#888888; font-size:14px;}");
    crossPlatformLabel->show();
}



void settingpage::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawPixmap(0, 0, QPixmap(":/picture/Settingpage/dialogbox.png"));
}


//打开文档
void settingpage::on_Help_clicked()
{
    QFile file(":/doc/Exp.doc");
    QTemporaryDir tempDir;
    tempDir.setAutoRemove(false);
    if (tempDir.isValid()) {
        QString tempFile = tempDir.path() + "/Exp.doc";
        if (QFile::copy(":/doc/Exp.doc", tempFile)) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(tempDir.path() + "/Exp.doc"));
        }
    }
}


//打开网页
void settingpage::on_Web_clicked()
{
    QDesktopServices::openUrl(QUrl(QLatin1String("https://gitee.com/guyi2020/bejeweled-2021")));
}

//关闭设置界面
void settingpage::on_Done_clicked()
{
    this->setVisible(false);
}


settingpage::~settingpage()
{
    delete ui;
}



void settingpage::on_LangB_clicked()
{
    if(index == 0){
        ui->LangB->setStyleSheet("border-image:url(:/picture/Settingpage/Toggle_2.png)");
        ui->LangL->setText("简体中文");
        ui->label_Bright->setText("亮度");
        ui->label_Bright->setGeometry(190,200,101,31);
        ui->label_volume->setText("音量");
        ui->label_settings->setText("设置");
        ui->label_settings->setGeometry(320,30,121,51);
        ui->label_help->setText("帮助");
        ui->label_done->setText("退出");
        ui->label_web->setText("网页");
        selectLan(index);
        index = 1;
        return;
    }else{
        ui->LangB->setStyleSheet("border-image:url(:/picture/Settingpage/Toggle_1.png)");
        ui->LangL->setText("English");
        ui->label_Bright->setText("Brightness");
        ui->label_volume->setText("Volume");
        ui->label_settings->setText("Settings");
        ui->label_help->setText("Help");
        ui->label_done->setText("Done");
        ui->label_web->setText("Web");
        selectLan(index);
        index = 0;
        return;
    }
}
