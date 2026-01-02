#ifndef GEM_H
#define GEM_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QMovie>
#include <QDebug>

#define LEN 118

class Gem:public QPushButton
{
    Q_OBJECT
public:
    QString path_stable[100];
    QString path_dynamic[100];
    void initialPath();
    int type;
    int x, y;
    void setY(int num){y=num;}

    int oriX,oriY;
    explicit Gem(int type, int len, int x, int y, QWidget *parent = nullptr, int offset=0);
    void bomb();
    QMovie* spinGif;
    QLabel* spinLabel;
    QMovie* magicGif;
    QLabel* magicLabel;
    QLabel* normalLabel;

    bool isHighlighted; // 新增：标记是否高亮
    void setHighlight(bool highlight); // 新增：设置高亮状态
    QLabel* specialBorder = nullptr; // 特殊宝石边框
    int specialType = 0;  // 特殊宝石类型：0=普通，1=炸弹，2=十字消，3=时间球

private:

signals:
    void mouseClickedGem(Gem*);
};

#endif // GEM_H
