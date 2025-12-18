#ifndef REGISTERWIDGET_H
#define REGISTERWIDGET_H

#include <QWidget>
#include <QPushButton>

namespace Ui {
class RegisterWidget;
}

class RegisterWidget : public QWidget
{
    Q_OBJECT

signals:
    void backButtonClicked();

public:
    explicit RegisterWidget(QWidget *parent = nullptr);
    ~RegisterWidget();

private:
    Ui::RegisterWidget *ui;
};

#endif // REGISTERWIDGET_H
