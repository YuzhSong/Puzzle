#ifndef RANKLISTPAGE_H
#define RANKLISTPAGE_H

#include <QWidget>
#include <QKeyEvent>
#include "hoverbutton.h"

namespace Ui {
class rankListPage;
}

class rankListPage : public QWidget
{
    Q_OBJECT

public:
    explicit rankListPage(QWidget *parent = nullptr);
    ~rankListPage();

    void setUserRankList(const QString &s);
    void setRankList(const QString &s);

protected:
    void keyPressEvent(QKeyEvent *ev) override;
    void showEvent(QShowEvent *event) override;

private slots:
    void onReturnButtonClicked();
    void onQueryButtonClicked();

private:
    Ui::rankListPage *ui;
    class Client *client;

    void Sleep(int msec);

signals:
    void showStartPage();
};

#endif // RANKLISTPAGE_H
