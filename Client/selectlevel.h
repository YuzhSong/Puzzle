#ifndef SELECTLEVEL_H
#define SELECTLEVEL_H

#include <QMainWindow>
#include<qpainter.h>
#include"settingpage.h"
#include"QLabel"
#include "client.h"

namespace Ui {
class selectlevel;
}

class selectlevel : public QMainWindow
{
    Q_OBJECT

public:
    explicit selectlevel(QWidget *parent = nullptr);
    ~selectlevel();

    void setClient(Client* client) { this->client = client; }  // 添加设置client的方法

    QLabel*level1L=new QLabel("Easy");
    QLabel*level2L=new QLabel("Medium");
    QLabel*level3L=new QLabel("Hard");
    QLabel*levelL=new QLabel("Level");
    QLabel*DoneL=new QLabel("Done");

    // 辅助函数：检查登录状态
    bool checkLogin();

private slots:
    void on_level1B_pressed();

    void on_level1B_released();

    void on_level2B_pressed();

    void on_level3B_pressed();

    void on_DoneB_pressed();

    void on_level2B_released();

    void on_level3B_released();

    void on_DoneB_released();

    void on_DoneB_clicked();

    void initColor();

private:
    Ui::selectlevel *ui;
    settingpage settingP;
    Client* client;
    QPixmap pixmap=QPixmap(":/picture/Settingpage/dialogbox_select.png").scaled(QSize(574,307));
    QPalette white;
    QPalette dyellow;
    QPalette purple;
    int level=4;
    void paintEvent(QPaintEvent *);

signals:
    void selectDone(int difficulty);
};

#endif // SELECTLEVEL_H
