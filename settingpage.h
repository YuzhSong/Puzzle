#ifndef SETTINGPAGE_H
#define SETTINGPAGE_H

#include <QMainWindow>
#include<QLabel>

namespace Ui {
class settingpage;
}

class settingpage : public QMainWindow
{
    Q_OBJECT

public:
    explicit settingpage(QWidget *parent = nullptr);
    ~settingpage();

private slots:
    void on_Help_clicked();
    void on_Web_clicked();
    void on_Done_clicked();
    void paintEvent(QPaintEvent *event);
    void on_LangB_clicked();

private:
    Ui::settingpage *ui;
    int index;  // 语言切换索引

signals:
    void selectLan(int index);
};

#endif // SETTINGPAGE_H
